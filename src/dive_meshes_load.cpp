#include "dive_meshes_load.hpp"
#include "dive_status.hpp"
#include "dive_elements.hpp"
#include "dive_elements_hexa.hpp"

#include "cgnslib.h"

#include <algorithm>
#include <memory>
#include <limits>
#include <array>

namespace dive {
	namespace meshes {

		bool IsEmpty(const String& line)
		{
			return utils::string::IsEmpty(line);
		};
		bool IsComment(const String& line)
		{
			if (line.size() >= 2)
			{
				if (line[0] == '*' && line[1] == '*')
				{
					return true;
				}
			}

			return false;
		};

		void LoadInpNodes(IMeshPtr mesh, const std::vector<Strings>& table)
		{
			Status status{ dive::DIVE_SUCCESS };
			NumberNodes numberNodes{ 0 };
			NumberNodes counter{ 0 };
			
			for (Index i = 0; i < table.size(); ++i)
			{
				if (utils::string::ToUpper(table[i][0]) == "*NODE")
				{
					for (Index j = i + 1; j < table.size(); ++j)
					{
						if (table[j][0][0] == '*')
						{
							break;
						}

						//TRY block to catch conversion errors
						auto nodeTag = utils::string::ConvertTo<Tag>(table[j][0]);
						auto x = utils::string::ConvertTo<Scalar>(table[j][1]);
						auto y = utils::string::ConvertTo<Scalar>(table[j][2]);
						auto z = utils::string::ConvertTo<Scalar>(table[j][3]);
						auto node = nodes::CreateNode(nodeTag, x, y, z);

						mesh->AddNode(node, status, false);
						if (status != dive::DIVE_SUCCESS)
						{
							logger::Error(dive::headerDive, "MESH INP node could not be added: " + dive::messages.at(status));
							return;
						}

						numberNodes++;
						i++;
					}
				}
			}
			
			mesh->SortNodesTag();
			logger::Info(dive::headerDive, "MESH INP nodes added: %u", numberNodes);
		}
		void LoadInpElements(IMeshPtr mesh, const std::vector<Strings>& table)
		{
			Status status{ dive::DIVE_SUCCESS };
			NumberElements numberElements{ 0 };
			Tag elementTag;

			for (Index i = 0; i < table.size(); ++i)
			{
				if (utils::string::ToUpper(table[i][0]) == "*ELEMENT")
				{
					elementTag = utils::string::ConvertTo<Tag>(table[i][0]);

					for (Index j = 0; j < table[i].size(); ++j)
					{

					}

					for (Index j = i + 1; j < table.size(); ++j)
					{


						std::cout << "Element tag / type: " << elementTag << " " << table[j][1] << std::endl;

						/*auto element = elements::CreateElementHexa(elementTag);
						element->SetOrder(elements::order_linear);
						element->SetParametric(elements::parametric_linear);
						element->SetNumberDof(mesh->GetNumberDof());

						for (NumberNodes k = 0; k < element->GetNumberNodes(); ++k)
						{
							auto nodeTag = utils::string::ConvertTo<Tag>(table[j][k + 1]);
							auto node = mesh->GetNodeSorted(nodeTag, status);
							element->SetNode(k, node);
						}

						mesh->AddElement(element, status, false);
						if (status != dive::DIVE_SUCCESS)
						{
							logger::Error(dive::headerDive, "Element could not be added: " + dive::messages.at(status));
							return;
						}
						numberElements++;*/
					}
				}
			}
			mesh->SortElementsTag();
			logger::Info(dive::headerDive, "Elements added: %u", numberElements);
		}
		IMeshPtr LoadInp(Tag meshTag, String fileName, NumberDof numberDof, Status& status)
		{
			TimerStart();
			String line{};
			Strings lines(0);
			Strings tokens(0);
			IMeshPtr mesh{ nullptr };
			
			utils::file::Text file;
			std::vector<Strings> table;

			status = file.Open(fileName);
			if (status != utils::file::UTILS_SUCCESS)
			{
				logger::Error(dive::headerDive, "MESH INP file could not be opened: " + dive::messages.at(dive::DIVE_FILE_NOT_OPENED));
				return mesh;
			}

			mesh = CreateMesh(meshTag);
			if (mesh == nullptr)
			{
				logger::Error(dive::headerDive, "MESH INP not possible to create: " + dive::messages.at(dive::DIVE_NULL_POINTER));
				return mesh;
			}
			
			auto stream = static_cast<std::istringstream>(file.GetFull());
			while (std::getline(stream, line))
			{
				if (IsEmpty(line) || IsComment(line))
				{
					continue;
				}

				lines.push_back(utils::string::RemoveEndSpaces(line));
			}

			for (Index i = 0; i < lines.size(); ++i)
			{
				line = lines[i];
				while(lines[i].back() == ',' && (i + 1) < lines.size())
				{
					++i;
					line += lines[i];
				}
	
				tokens = utils::string::Split(line, {','});
				table.push_back(tokens);
			}
		
			LoadInpNodes(mesh, table);
			//LoadInpElements(mesh, table);
			//LoadInpSelections(mesh, table);

			TimerElapsed(__FUNCTION__);

			return mesh;
		}
	} // namespace meshes
} // namespace dive