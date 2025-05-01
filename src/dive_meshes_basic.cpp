#include "dive_meshes_basic.hpp"
#include "dive_status.hpp"
#include "dive_elements.hpp"
#include "dive_elements_hexa.hpp"

#include <algorithm>
#include <memory>
#include <limits>

namespace dive {
	namespace meshes {
		MeshPtr CreateMesh()
		{
			auto res = Mesh::Create();

			return res;
		}
		MeshPtr CreateMesh(Tag meshTag)
		{
			auto res = Mesh::Create();

			res->SetTag(meshTag);

			return res;
		}
		MeshPtr Mesh::Create()
		{
			class MakeSharedEnabler : public Mesh
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		MeshPtr Mesh::GetPtr()
		{
			return std::dynamic_pointer_cast<Mesh>(shared_from_this());
		}
		ConstMeshPtr Mesh::GetPtr() const
		{
			return const_cast<Mesh*>(this)->GetPtr();
		}
		void Mesh::AddNode(INodePtr node, Status& status, bool IndexCheck)
		{
			if (node == nullptr)
			{
				status = DIVE_NULL_POINTER;
				return;
			}

			if (nodes_.size() == std::numeric_limits<Index>::max() - 1)
			{
				status = DIVE_MAX_OF_RANGE;
				return;
			}

			if (IndexCheck)
			{
				auto index = GetIndexNode(node->GetTag(), status);


				if (status == DIVE_SUCCESS)
				{
					status = DIVE_ALREADY_EXISTS;
					return;
				}
			}

			nodes_.push_back(node);
			status = DIVE_SUCCESS;
		}
		void Mesh::AddElement(IElementPtr element, Status& status, bool IndexCheck)
		{
			if (element == nullptr)
			{
				status = DIVE_NULL_POINTER;
				return;
			}

			if (elements_.size() == std::numeric_limits<Index>::max() - 1)
			{
				status = DIVE_MAX_OF_RANGE;
				return;
			}

			if (IndexCheck)
			{
				auto index = GetIndexElement(element->GetTag(), status);

				if (status == DIVE_SUCCESS)
				{
					status = DIVE_ALREADY_EXISTS;
					return;
				}
			}

			elements_.push_back(element);
			status = DIVE_SUCCESS;
		}
		void Mesh::RemoveNode(INodePtr node, Status& status)
		{
			if (node == nullptr)
			{
				status = DIVE_NULL_POINTER;
				return;
			}

			auto index = GetIndexNode(node->GetTag(), status);

			if (status == DIVE_NOT_FOUND)
			{
				return;
			}

			nodes_.erase(nodes_.begin() + index);
			status = DIVE_SUCCESS;
		}
		void Mesh::RemoveNode(Tag tag, Status& status)
		{
			auto index = GetIndexNode(tag, status);

			if (status == DIVE_NOT_FOUND)
			{
				return;
			}

			auto ptr = nodes_[index];

			nodes_.erase(nodes_.begin() + index);
			status = DIVE_SUCCESS;
		}
		void Mesh::RemoveElement(IElementPtr element, Status& status)
		{
			if (element == nullptr)
			{
				status = DIVE_NULL_POINTER;
				return;
			}

			auto index = GetIndexElement(element->GetTag(), status);

			if (status == DIVE_NOT_FOUND)
			{
				return;
			}

			auto ptr = elements_[index];

			elements_.erase(elements_.begin() + index);
			status = DIVE_SUCCESS;
		}
		void Mesh::RemoveElement(Tag tag, Status& status)
		{
			auto index = GetIndexElement(tag, status);

			if (status == DIVE_NOT_FOUND)
			{
				return;
			}

			auto ptr = elements_[index];

			elements_.erase(elements_.begin() + index);
			status = DIVE_SUCCESS;
		}
		const Nodes& Mesh::GetNodes() const
		{
			return nodes_;
		}
		const Elements& Mesh::GetElements() const
		{
			return elements_;
		}
		INodePtr Mesh::GetNode(Tag tag, Status& status) const
		{
			INodePtr res{ nullptr };

			auto index = GetIndexNode(tag, status);
			if (status != DIVE_SUCCESS)
			{
				return res;
			}

			res = nodes_[index];

			return res;
		}
		INodePtr Mesh::GetNodeIndex(NodeIndex index, Status& status) const
		{
			INodePtr res{ nullptr };

			if (index >= nodes_.size())
			{
				status = DIVE_OUT_OF_RANGE;
				return res;
			}

			if (nodes_[index] == nullptr)
			{
				status = DIVE_NULL_POINTER;
				return res;
			}

			status = DIVE_SUCCESS;
			res = nodes_[index];

			return res;
		}
		INodePtr Mesh::GetNodeSorted(Tag tag, Status& status) const
		{
			INodePtr res{ nullptr };

			auto index = GetIndexNodeSorted(tag, status);
			if (status != DIVE_SUCCESS)
			{
				return res;
			}

			res = nodes_[index];

			return res;
		}
		IElementPtr Mesh::GetElement(Tag tag, Status& status) const
		{
			IElementPtr res{ nullptr };

			auto index = GetIndexElement(tag, status);
			if (status != DIVE_SUCCESS)
			{
				return res;
			}

			res = elements_[index];

			return res;
		}
		IElementPtr Mesh::GetElementIndex(ElementIndex index, Status& status) const
		{
			IElementPtr res{ nullptr };

			if (index >= elements_.size())
			{
				status = DIVE_OUT_OF_RANGE;
				return res;
			}

			if (elements_[index] == nullptr)
			{
				status = DIVE_NULL_POINTER;
				return res;
			}

			status = DIVE_SUCCESS;
			res = elements_[index];

			return res;
		}
		IElementPtr Mesh::GetElementSorted(Tag tag, Status& status) const
		{
			IElementPtr res{ nullptr };

			auto index = GetIndexElementSorted(tag, status);
			if (status != DIVE_SUCCESS)
			{
				return res;
			}

			res = elements_[index];

			return res;
		}
		Scalar Mesh::GetElementSizeMinimum() const
		{
			Scalar size{ 0.0 };
			Scalar res{ std::numeric_limits<Scalar>::infinity() };

			for (auto& element : elements_)
			{
				size = element->Size();

				if (size < res)
				{
					res = size;
				}
			}

			return res;
		}
		NumberDof Mesh::GetTotalDof()
		{
			return totalDof_;
		}
		Index Mesh::GetPivot()
		{
			return pivot_;
		}
		Tag Mesh::GetTag() const
		{
			return tag_;
		}
		void Mesh::SetTag(Tag tag)
		{
			tag_ = tag;
		}
		void Mesh::SortNodesTag()
		{
			std::sort(nodes_.begin(), nodes_.end(), 
				[&](dive::INodePtr ptr1, dive::INodePtr ptr2) -> bool
				{
					return ptr1->GetTag() < ptr2->GetTag();
				});
		}
		void Mesh::SortElementsTag()
		{
			std::sort(elements_.begin(), elements_.end(), 
				[&](dive::IElementPtr ptr1, dive::IElementPtr ptr2) -> bool
				{
					return ptr1->GetTag() < ptr2->GetTag();
				});
		}
		Index Mesh::GetIndexNode(Tag tag, Status& status) const
		{
			auto it = std::find_if(nodes_.begin(), nodes_.end(), 
					[&](INodePtr ptr1) -> bool
					{
						return ptr1->GetTag() == tag;
					});

			if (it == nodes_.end())
			{
				status = DIVE_NOT_FOUND;
				return std::numeric_limits<Index>::max();
			}

			status = DIVE_SUCCESS;
			return std::distance(nodes_.begin(), it);
		}
		Index Mesh::GetIndexNodeSorted(Tag tag, Status& status) const
		{
			auto it = std::lower_bound(nodes_.begin(), nodes_.end(), tag,
				[&](INodePtr ptr, Tag value) -> bool
				{
					return ptr->GetTag() < value;
				});

			if (it == nodes_.end())
			{
				status = DIVE_NOT_FOUND;
				return std::numeric_limits<Index>::max();
			}

			status = DIVE_SUCCESS;
			return std::distance(nodes_.begin(), it);
		}
		Index Mesh::GetIndexElement(Tag tag, Status& status) const
		{
			auto it = std::find_if(elements_.begin(), elements_.end(),
				[&](IElementPtr ptr1) -> bool
				{
					return ptr1->GetTag() == tag;
				});

			if (it == elements_.end())
			{
				status = DIVE_NOT_FOUND;
				return std::numeric_limits<Index>::max();
			}

			status = DIVE_SUCCESS;
			return std::distance(elements_.begin(), it);
		}
		Index Mesh::GetIndexElementSorted(Tag tag, Status& status) const
		{
			auto it = std::lower_bound(elements_.begin(), elements_.end(), tag,
				[&](IElementPtr ptr, Tag value) -> bool
				{
					return ptr->GetTag() < value;
				});

			if (it == elements_.end())
			{
				status = DIVE_NOT_FOUND;
				return std::numeric_limits<Index>::max();
			}

			status = DIVE_SUCCESS;
			return std::distance(elements_.begin(), it);
		}
	
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
			IElementPtr element{ nullptr };

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
						element = elements::CreateElementHexa(elementTag);
						std::dynamic_pointer_cast<elements::IShape>(element)->SetOrder(elements::order_linear);
						std::dynamic_pointer_cast<elements::IShape>(element)->SetParametric(elements::parametric_linear);
						element->SetNumberDof(numberDof);

						for (NumberNodes k = 0; k < element->GetNumberNodes(); ++k)
						{
							auto nodeTag = utils::string::ConvertTo<Tag>(split[lookUpTableType5[k] + 1]);
							auto node = mesh->GetNodeSorted(nodeTag, status);

							element->SetNode(k, node);
						}

						break;
					}
					case 17: //element_hexa20
					{
						element = elements::CreateElementHexa(elementTag);
						std::dynamic_pointer_cast<elements::IShape>(element)->SetOrder(elements::order_quadratic);
						std::dynamic_pointer_cast<elements::IShape>(element)->SetParametric(elements::parametric_quadratic);
						element->SetNumberDof(numberDof);

						for (NumberNodes k = 0; k < element->GetNumberNodes(); ++k)
						{
							auto nodeTag = utils::string::ConvertTo<Tag>(split[lookUpTableType17[k] + 1]);
							auto node = mesh->GetNodeSorted(nodeTag, status);

							element->SetNode(k, node);
						}

						break;
					}
					default:
						continue;
					}

					mesh->AddElement(element, status, false);
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

			input = file.GetFull();
			mesh = CreateMesh(meshTag);

			if (mesh == nullptr)
			{
				logger::Error(dive::headerDive, "Not possible to create mesh: " + dive::messages.at(dive::DIVE_NULL_POINTER));
				return mesh;
			}

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

		void DeformByInterpolation(IMeshPtr mesh, IInterpolationPtr interpolation)
		{
			std::size_t rows;
			Vector point;
			Vector disp;
			Matrix mat;
			INodePtr aux{ nullptr };

			auto& nodes = mesh->GetNodes();

			for (auto& node : nodes)
			{
				point = node->GetPoint();

				aux = nodes::CreateNode();
				aux->SetPoint(point);

				interpolation->GetValue(aux);

				mat = aux->GetValue();
				rows = mat.GetRows();
				disp.Resize(rows);

				for (unsigned int i = 0; i < rows; ++i)
				{
					disp(i) = mat(i, 0);
				}

				node->SetPoint(point + disp);
			}
		}

	} // namespace meshes
} // namespace dive