#include "dive_meshes_load.hpp"
#include "dive_status.hpp"
#include "dive_elements.hpp"
#include "dive_elements_hexa.hpp"

#include "cgnslib.h"

#include <algorithm>
#include <memory>
#include <limits>

namespace dive {
	namespace meshes {
		const NodeIndex	lookUpTableType5[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
		const NodeIndex	lookUpTableType17[20] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 11, 13, 9, 16, 18, 19, 17, 10, 12, 14, 15 };

		void LoadGmshFormat(IMeshPtr mesh, const Strings& data)
		{
			auto split = utils::string::Split(data[0]);

			if (split[0] != "4.1")
			{
				logger::Error(dive::headerDive, "Invalid file format: " + dive::messages.at(dive::DIVE_INVALID_FORMAT));
				return;
			}

			logger::Info(dive::headerDive, "File version: " + split[0]);
			logger::Info(dive::headerDive, "File format: " + split[1]);
			logger::Info(dive::headerDive, "Data size: " + split[2]);
		}
		void LoadGmshNodes(IMeshPtr mesh, const Strings& data)
		{
			Status status{ dive::DIVE_SUCCESS };

			unsigned int counter{ 0 };

			auto split = utils::string::Split(data[0]);
			auto numEntityBlocks = utils::string::ConvertTo<unsigned int>(split[0]);
			auto numNodes = utils::string::ConvertTo<unsigned int>(split[1]);
			auto minNodeTag = utils::string::ConvertTo<unsigned int>(split[2]);
			auto maxNodeTag = utils::string::ConvertTo<unsigned int>(split[3]);

			for (std::size_t i = 1; i < data.size(); i++)
			{
				split = utils::string::Split(data[i]);

				auto entityDim = utils::string::ConvertTo<int>(split[0]);
				auto entityTag = utils::string::ConvertTo<int>(split[1]);
				auto parametric = utils::string::ConvertTo<int>(split[2]);
				auto numNodesInBlock = utils::string::ConvertTo<unsigned int>(split[3]);

				for (unsigned int j = 0; j < numNodesInBlock; j++)
				{
					split = utils::string::Split(data[i + j + numNodesInBlock + 1]);

					auto x = utils::string::ConvertTo<Scalar>(split[0]);
					auto y = utils::string::ConvertTo<Scalar>(split[1]);
					auto z = utils::string::ConvertTo<Scalar>(split[2]);
					auto nodeTag = utils::string::ConvertTo<dive::Tag>(data[i + j + 1]);
					auto node = nodes::CreateNode(nodeTag, x, y, z);

					mesh->AddNode(node, status, false);

					if (status != dive::DIVE_SUCCESS)
					{
						logger::Error(dive::headerDive, "Node could not be added: " + dive::messages.at(status));
						return;
					}

					counter++;
				}

				i += 2 * numNodesInBlock;
			}

			mesh->SortNodesTag();

			logger::Info(dive::headerDive, "Nodes added: %u", counter);
		}
		void LoadGmshElements(IMeshPtr mesh, const Strings& data, NumberDof numberDof)
		{
			Status status{ dive::DIVE_SUCCESS };

			Tag	elementTag;

			unsigned int counter{ 0 };

			auto split = utils::string::Split(data[0]);

			auto numEntityBlocks = utils::string::ConvertTo<unsigned int>(split[0]);
			auto numElements = utils::string::ConvertTo<unsigned int>(split[1]);
			auto minElementTag = utils::string::ConvertTo<unsigned int>(split[2]);
			auto maxElementTag = utils::string::ConvertTo<unsigned int>(split[3]);

			for (std::size_t i = 1; i < data.size(); i++)
			{
				split = utils::string::Split(data[i]);

				auto entityDim = utils::string::ConvertTo<int>(split[0]);
				auto entityTag = utils::string::ConvertTo<int>(split[1]);
				auto elementType = utils::string::ConvertTo<int>(split[2]);
				auto numElementsInBlock = utils::string::ConvertTo<unsigned int>(split[3]);

				for (unsigned int j = 0; j < numElementsInBlock; j++)
				{
					split = utils::string::Split(data[i + j + 1]);
					elementTag = utils::string::ConvertTo<Tag>(data[i + j + 1]);

					switch (elementType)
					{
					case 5: // element_hexa8
					{
						auto element = elements::CreateElementHexa(elementTag);
						element->SetOrder(elements::order_linear);
						element->SetParametric(elements::parametric_linear);
						element->SetNumberDof(numberDof);

						for (NumberNodes k = 0; k < element->GetNumberNodes(); ++k)
						{
							auto nodeTag = utils::string::ConvertTo<Tag>(split[lookUpTableType5[k] + 1]);
							auto node = mesh->GetNodeSorted(nodeTag, status);

							element->SetNode(k, node);
						}

						mesh->AddElement(element, status, false);

						break;
					}
					case 17: //element_hexa20
					{
						auto element = elements::CreateElementHexa(elementTag);
						element->SetOrder(elements::order_quadratic);
						element->SetParametric(elements::parametric_quadratic);
						element->SetNumberDof(numberDof);

						for (NumberNodes k = 0; k < element->GetNumberNodes(); ++k)
						{
							auto nodeTag = utils::string::ConvertTo<Tag>(split[lookUpTableType17[k] + 1]);
							auto node = mesh->GetNodeSorted(nodeTag, status);

							element->SetNode(k, node);
						}

						mesh->AddElement(element, status, false);

						break;
					}
					default:
						continue;
					}

					if (status != dive::DIVE_SUCCESS)
					{
						logger::Error(dive::headerDive, "Element could not be added: " + dive::messages.at(status));
						return;
					}

					counter++;
				}

				i += numElementsInBlock;
			}

			mesh->SortElementsTag();

			logger::Info(dive::headerDive, "Elements added: %u", counter);
		}
		IMeshPtr LoadGmsh(Tag meshTag, String fileName, NumberDof numberDof, Status& status)
		{
			TimerStart();

			String input;
			String	line;
			Strings format;
			Strings nodes;
			Strings elements;
			IMeshPtr mesh{ nullptr };
			utils::file::Text file;

			status = file.Open(fileName);
			if (status != utils::file::UTILS_SUCCESS)
			{
				logger::Error(dive::headerDive, "File could not be opened: " + dive::messages.at(dive::DIVE_FILE_NOT_OPENED));
				return mesh;
			}

			mesh = CreateMesh(meshTag);
			if (mesh == nullptr)
			{
				logger::Error(dive::headerDive, "Not possible to create mesh: " + dive::messages.at(dive::DIVE_NULL_POINTER));
				return mesh;
			}

			input = file.GetFull();
			input.erase(std::remove(input.begin(), input.end(), '\r'), input.end());
			std::istringstream aux(input);

			while (std::getline(aux, line))
			{
				if (line == "$MeshFormat")
				{

					while (std::getline(aux, line))
					{
						if (line == "$EndMeshFormat")
						{
							break;
						}
						format.push_back(line);
					}
				}
				else if (line == "$Nodes")
				{
					while (std::getline(aux, line))
					{
						if (line == "$EndNodes")
						{
							break;
						}
						nodes.push_back(line);
					}
				}
				else if (line == "$Elements")
				{
					while (std::getline(aux, line))
					{
						if (line == "$EndElements")
						{
							break;
						}
						elements.push_back(line);
					}
				}
			}

			LoadGmshFormat(mesh, format);
			LoadGmshNodes(mesh, nodes);
			LoadGmshElements(mesh, elements, numberDof);

			TimerElapsed(__FUNCTION__);

			return mesh;
		}

		constexpr unsigned int CGNS_MAX_NAME_LENGTH = 256;

		struct Coordinate
		{
			int index;
			
			String name;			
			DataType_t type;
		};
		using Coordinates = std::vector<Coordinate>;

		struct Zone 
		{
			int index;
			int numberOfGrids;
			int numberOfCoordinates;

			String name;
			ZoneType_t type;
			cgsize_t size;
		};
		using Zones = std::vector<Zone>;

		void LoadCGNSNodes(IMeshPtr& mesh, int fileHandler, int baseIndex, Zone& zone)
		{
			Status status{ dive::DIVE_SUCCESS };
			Scalars data(zone.size);
			cgsize_t cmin{ 1 };
			cgsize_t cmax{ zone.size };
			Coordinates coordinates(zone.numberOfCoordinates);

			for (auto j = cmin; j <= cmax; ++j)
			{
				auto node = nodes::CreateNode(j, 0.0, 0.0, 0.0);
				mesh->AddNode(node, status, false);

				if (status != dive::DIVE_SUCCESS)
				{
					logger::Error(dive::headerDive, "Node could not be added: " + dive::messages.at(status));
					return;
				}
			}

			for (int i = 0; i < zone.numberOfCoordinates; ++i)
			{
				char name[CGNS_MAX_NAME_LENGTH];

				coordinates[i].index = i + 1;
				if (cg_coord_info(fileHandler, baseIndex, zone.index, i + 1, &coordinates[i].type, name)) cg_error_exit();
				logger::Info(dive::headerDive, "Zone coordinate name: %s", name);
				logger::Info(dive::headerDive, "Zone coordinate type: %d", coordinates[i].type);
				coordinates[i].name = String(name);

				if (coordinates[i].type != CGNS_ENUMV(RealDouble))
				{
					logger::Error(dive::headerDive, "Zone coordinate type is not RealDouble: " + dive::messages.at(dive::DIVE_INVALID_FORMAT));
					cg_error_exit();
				}

				if (cg_coord_read(fileHandler, baseIndex, zone.index, coordinates[i].name.c_str(), coordinates[i].type, &cmin, &cmax, &data[0])) cg_error_exit();
				logger::Info(dive::headerDive, "Zone coordinate size: %d", zone.size);

				auto& nodes = mesh->GetNodes();
				if (coordinates[i].name == "CoordinateX")
				{
					for (int j = 0; j < cmax; ++j)
					{
						auto point = nodes[j]->GetPoint();
						point(0) = data[j];
						nodes[j]->SetPoint(point);
					}
				}
				else if (coordinates[i].name == "CoordinateY")
				{
					for (int j = 0; j < cmax; ++j)
					{
						auto point = nodes[j]->GetPoint();
						point(1) = data[j];
						nodes[j]->SetPoint(point);
					}
				}
				else if (coordinates[i].name == "CoordinateZ")
				{
					for (int j = 0; j < cmax; ++j)
					{
						auto point = nodes[j]->GetPoint();
						point(2) = data[j];
						nodes[j]->SetPoint(point);
					}
				}
			}

			mesh->SortNodesTag();
		}
		void LoadCGNSElements(IMeshPtr& mesh, int fileHandler, int baseIndex, Zone& zone, NumberDof numberDof)
		{
			Status status{ dive::DIVE_SUCCESS };
			ElementType_t elementType;
			cgsize_t emin{ 0 };
			cgsize_t emax{ 0 };

			int numberSections{ 0 };
			int boundary;
			int parentFlag;
			
			char name[CGNS_MAX_NAME_LENGTH];

			if (cg_nsections(fileHandler, baseIndex, zone.index, &numberSections)) cg_error_exit();
			logger::Info(dive::headerDive, "Zone number of sections: %d", numberSections);

			for (int i = 0; i < numberSections; i++)
			{
				if (cg_section_read(fileHandler, baseIndex, zone.index, i + 1, name, &elementType, &emin, &emax, &boundary, &parentFlag)) cg_error_exit();

				switch (elementType)
				{
				case HEXA_8: // element_hexa8
				{
					auto element = elements::CreateElementHexa(Tag(i + 1));
					element->SetOrder(elements::order_linear);
					element->SetParametric(elements::parametric_linear);
					element->SetNumberDof(numberDof);

					std::vector<cgsize_t> connectivity(element->GetNumberNodes());

					if(cg_elements_read(fileHandler, baseIndex, zone.index, i + 1, &connectivity[0], NULL)) cg_error_exit();

					for (NumberNodes k = 0; k < element->GetNumberNodes(); ++k)
					{
						auto nodeTag = connectivity[k];
						auto node = mesh->GetNodeSorted(nodeTag, status);

						element->SetNode(k, node);
					}

					mesh->AddElement(element, status, false);

					break;
				}
				case HEXA_20: //element_hexa20
				{
					auto element = elements::CreateElementHexa(Tag(i + 1));
					element->SetOrder(elements::order_quadratic);
					element->SetParametric(elements::parametric_quadratic);
					element->SetNumberDof(numberDof);

					std::vector<cgsize_t> connectivity(element->GetNumberNodes());

					if (cg_elements_read(fileHandler, baseIndex, zone.index, i + 1, &connectivity[0], NULL)) cg_error_exit();

					for (NumberNodes k = 0; k < element->GetNumberNodes(); ++k)
					{
						auto nodeTag = connectivity[k];
						auto node = mesh->GetNodeSorted(nodeTag, status);

						element->SetNode(k, node);
					}

					mesh->AddElement(element, status, false);
					break;
				}
				default:
					continue;
				}
				
			}
		}
		Zone LoadCGNSZone(IMeshPtr& mesh, int fileHandler, int baseIndex, int zoneIndex)
		{
			Zone zone;

			zone.index = zoneIndex;
			zone.name.resize(CGNS_MAX_NAME_LENGTH);

			if (cg_zone_type(fileHandler, baseIndex, zone.index, &zone.type)) cg_error_exit();
			if (cg_zone_read(fileHandler, baseIndex, zone.index, &zone.name[0], &zone.size)) cg_error_exit();
			logger::Info(dive::headerDive, "Zone name: %s", zone.name.c_str());
			logger::Info(dive::headerDive, "Zone index: %d", zone.index);
			logger::Info(dive::headerDive, "Zone type: %d", zone.type);
			logger::Info(dive::headerDive, "Zone size: %d", zone.size);

			if (cg_ngrids(fileHandler, baseIndex, zone.index, &zone.numberOfGrids)) cg_error_exit();
			logger::Info(dive::headerDive, "Zone number of grids: %d", zone.numberOfGrids);

			if (cg_ncoords(fileHandler, baseIndex, zone.index, &zone.numberOfCoordinates)) cg_error_exit();
			logger::Info(dive::headerDive, "Zone number of coordinates: %d", zone.numberOfCoordinates);
	
			return zone;
		}
		IMeshPtr LoadCGNS(Tag meshTag, String fileName, NumberDof numberDof, Status& status)
		{
			TimerStart();
			
			int fileType{ 0 };
			int fileHandler{ 0 };
			int numberBases{ 0 };
			int numberZones{ 0 };

			float version{ 0.0f };

			Zones zones;
			IMeshPtr mesh{ nullptr };

			mesh = CreateMesh(meshTag);
			if (mesh == nullptr)
			{
				logger::Error(dive::headerDive, "Not possible to create mesh: " + dive::messages.at(dive::DIVE_NULL_POINTER));
				return mesh;
			}

			status = cg_is_cgns(fileName.c_str(), &fileType);
			if(status != CG_OK)
			{
				logger::Error(dive::headerDive, "File is not a CGNS file: " + dive::messages.at(dive::DIVE_INVALID_FORMAT));
				cg_error_exit();
				return mesh;
			}

			status = cg_open(fileName.c_str(), CG_MODE_READ, &fileHandler);
			if(status != CG_OK)
			{
				logger::Error(dive::headerDive, "File could not be opened: " + dive::messages.at(dive::DIVE_FILE_NOT_OPENED));
				cg_error_exit();
				return mesh;
			}

			if (cg_version(fileHandler, &version)) cg_error_exit();
			logger::Info(dive::headerDive, "CGNS version: %.2f", version);

			if (cg_nbases(fileHandler, &numberBases)) cg_error_exit();
			logger::Info(dive::headerDive, "CGNS mesh number of bases: %d", numberBases);
			
			if (cg_nzones(fileHandler, numberBases, &numberZones)) cg_error_exit();
			logger::Info(dive::headerDive, "CGNS mesh number of zones: %d", numberZones);
			
			zones.resize(numberZones);
			for (int i = 0; i < numberZones; ++i)
			{
				zones[i] = LoadCGNSZone(mesh, fileHandler, 1, i + 1);

				LoadCGNSNodes(mesh, fileHandler, 1, zones[i]);
				LoadCGNSElements(mesh, fileHandler, 1, zones[i], numberDof);
			}

			cg_close(fileHandler);
			logger::Info(dive::headerDive, "CGNS mesh loading finished");

			TimerElapsed(__FUNCTION__);
			return mesh;
		}
	} // namespace meshes
} // namespace dive