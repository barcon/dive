#include "dive_problems.hpp"

#include "dive_loads_boundary_condition.hpp"
#include "dive_loads_distributed_volume.hpp"
#include "dive_loads_distributed_face.hpp"
#include "dive_loads_distributed_edge.hpp"
#include "dive_loads_node.hpp"
#include "dive_loads_node_transient.hpp"
#include "dive_selection.hpp"
#include "dive_status.hpp"

#include <exception>
#include <typeinfo>
#include <stdexcept>

namespace dive
{
	namespace problems
	{
		Sparse IntegralForm(IWeakFormElementPtr weakForm, IProblemPtr problem1, IProblemPtr problem2)
		{
			const auto& elements1 = problem1->GetMesh()->GetElements();
			const auto& elements2 = problem2->GetMesh()->GetElements();

			const auto& nodeMeshIndices1 = problem1->GetNodeMeshIndices();
			const auto& nodeMeshIndices2 = problem2->GetNodeMeshIndices();

			Sparse global(problem1->GetTotalDof(), problem2->GetTotalDof());
			Matrix local;

			for (ElementIndex i = 0; i < elements1.size(); ++i)
			{
				auto numberNodes1 = elements1[i]->GetNumberNodes();
				auto numberNodes2 = elements2[i]->GetNumberNodes();

				auto numberDof1 = elements1[i]->GetNumberDof();
				auto numberDof2 = elements2[i]->GetNumberDof();

				if (local.GetRows() != numberNodes1 * numberDof1 || local.GetCols() != numberNodes2 * numberDof2)
				{
					local.Resize(numberNodes1 * numberDof1, numberNodes2 * numberDof2);
				}

				std::dynamic_pointer_cast<elements::IElementMapped>(elements1[i])->IntegralWeakFormElement(weakForm, local);

				for (NodeIndex m = 0; m < numberNodes1; ++m)
				{
					for (NodeIndex n = 0; n < numberNodes2; ++n)
					{
						for (DofIndex dof1 = 0; dof1 < numberDof1; ++dof1)
						{
							for (DofIndex dof2 = 0; dof2 < numberDof2; ++dof2)
							{
								auto aux = global.GetValue(nodeMeshIndices1[i][m].dofIndices[dof1], nodeMeshIndices2[i][n].dofIndices[dof2]);
								aux += local.GetValue(m * numberDof1 + dof1, n * numberDof2 + dof2);

								global.SetValue(nodeMeshIndices1[i][m].dofIndices[dof1], nodeMeshIndices2[i][n].dofIndices[dof2], aux);
							}
						}
					}
				}
			}

			return global;
		}
		Vector IntegralForm(IWeakFormLoadPtr weakForm, IProblemPtr problem1, const Loads& loads, Scalar time)
		{
			Vector global(problem1->GetTotalDof(), 0.0);
			Vector local;
			Scalar aux{ 0 };
			Tag nodeTag{ 0 };
			NumberDof numberDof{ 0 };
			NumberNodes numberNodes{ 0 };
			ElementIndex elementIndex{ 0 };

			const auto& nodeMeshIndices = problem1->GetNodeMeshIndices();
			const auto& dofMeshIndices = problem1->GetDofMeshIndices();

			for (auto& load : loads)
			{
				if (load->GetType() == loads::load_node)
				{
					const auto& forceNode = std::static_pointer_cast<loads::ILoadNode>(load);

					local = forceNode->GetValue();
					numberDof = forceNode->GetNode()->GetNumberDof();

					for (DofIndex j = 0; j < numberDof; ++j)
					{
						auto globalIndex = forceNode->GetNode()->GetConnectivity().globalDofIndices[j];

						aux = global.GetValue(globalIndex);
						aux += local.GetValue(j);

						global.SetValue(globalIndex, aux);
					}
				}
				else if (load->GetType() == loads::load_node_transient)
				{
					const auto& forceNode = std::static_pointer_cast<loads::ILoadNodeTransient>(load);

					local = forceNode->GetValue(time);
					numberDof = forceNode->GetNode()->GetNumberDof();

					for (DofIndex j = 0; j < numberDof; ++j)
					{
						auto globalIndex = forceNode->GetNode()->GetConnectivity().globalDofIndices[j];

						aux = global.GetValue(globalIndex);
						aux += local.GetValue(j);

						global.SetValue(globalIndex, aux);
					}
				}
				else
				{
					if (load->GetType() == loads::load_distributed_volume)
					{
						const auto& element = std::dynamic_pointer_cast<loads::ILoadDistributedVolume>(load)->GetElement();

						element->IntegralWeakFormLoad(weakForm, load, local);
						numberDof = element->GetNumberDof();
						numberNodes = element->GetNumberNodes();
						elementIndex = element->GetElementIndex();
					}
					else if (load->GetType() == loads::load_distributed_face)
					{
						const auto& element = std::static_pointer_cast<loads::ILoadDistributedFace>(load)->GetElement();

						element->IntegralWeakFormLoad(weakForm, load, local);
						numberDof = element->GetNumberDof();
						numberNodes = element->GetNumberNodes();
						elementIndex = element->GetElementIndex();
					}
					else if (load->GetType() == loads::load_distributed_edge)
					{
						const auto& element = std::static_pointer_cast<loads::ILoadDistributedEdge>(load)->GetElement();

						element->IntegralWeakFormLoad(weakForm, load, local);
						numberDof = element->GetNumberDof();
						numberNodes = element->GetNumberNodes();
						elementIndex = element->GetElementIndex();
					}
					
					for (NodeIndex i = 0; i < numberNodes; ++i)
					{
						for (DofIndex j = 0; j < numberDof; ++j)
						{
							aux = global.GetValue(nodeMeshIndices[elementIndex][i].dofIndices[j]);
							aux += local.GetValue(i * numberDof + j);

							global.SetValue(nodeMeshIndices[elementIndex][i].dofIndices[j], aux);
						}
					}
				}
			}

			return global;
		}

		void UpdateMeshElements(IMeshPtr mesh, NumberDof numberDof)
		{
			TimerStart();

			mesh->SortElementsTag();

			const auto& elements = mesh->GetElements();

			for (ElementIndex i = 0; i < elements.size(); ++i)
			{
				auto element = std::dynamic_pointer_cast<elements::IElement>(elements[i]);

				element->SetNumberDof(numberDof);
				element->SetElementIndex(i);
				
				if (element->IsCacheable())
				{
					element->InitializeCache();
				}
			}

			TimerElapsed(__FUNCTION__);
		}
		void UpdateDofMeshIndices(IMeshPtr mesh, NumberDof& totalDof, DofMeshIndices& dofMeshIndices)
		{
			TimerStart();

			mesh->SortNodesTag();

			const auto& nodes = mesh->GetNodes();

			totalDof = 0;
			dofMeshIndices.clear();
			dofMeshIndices.reserve(nodes.size() * nodes[0]->GetNumberDof());

			for (NodeIndex i = 0; i < nodes.size(); ++i)
			{
				nodes[i]->GetConnectivity().nodeIndex = i;

				for (Index j = 0; j < nodes[i]->GetNumberDof(); ++j)
				{
					DofMeshIndex dofMeshIndex;

					dofMeshIndex.node = nodes[i];
					dofMeshIndex.nodeIndex = i;
					dofMeshIndex.dofIndex = j;
					dofMeshIndex.globalIndex = totalDof;
					dofMeshIndex.dirichlet = nullptr;
					dofMeshIndex.dirichletIndex = 0;
					dofMeshIndex.dirichletReordered = false;

					dofMeshIndices.push_back(dofMeshIndex);
					totalDof++;
				}
			}

			SortDofMeshIndices(dofMeshIndices);

			TimerElapsed(__FUNCTION__);
		}
		void UpdateNodeMeshIndices(IMeshPtr mesh, const DofMeshIndices& dofMeshIndices, NodeMeshIndices& nodeMeshIndices)
		{
			TimerStart();

			NodeMeshIndex nodeMeshIndex;

			INodePtr node;
			Tag nodeTag;

			std::vector<NodeMeshIndex> aux;

			const auto& elements = mesh->GetElements();

			nodeMeshIndices.clear();
			nodeMeshIndices.reserve(elements.size());

			for (NumberElements i = 0; i < elements.size(); ++i)
			{
				aux.clear();

				for (NumberNodes j = 0; j < elements[i]->GetNumberNodes(); ++j)
				{
					node = elements[i]->GetNode(j);
					nodeTag = node->GetTag();

					nodeMeshIndex.node = node;
					nodeMeshIndex.index = node->GetConnectivity().nodeIndex;
					nodeMeshIndex.dofIndices.clear();

					auto dofIndex = std::lower_bound(dofMeshIndices.begin(), dofMeshIndices.end(), nodeTag,
						[&](DofMeshIndex dofMeshIndex, Tag tag) -> bool
						{
							return (dofMeshIndex.node->GetTag() < tag);
						});

					for (NumberDof k = 0; k < node->GetNumberDof(); ++k)
					{
						nodeMeshIndex.dofIndices.push_back((*dofIndex).globalIndex);
						dofIndex++;
					}

					aux.push_back(nodeMeshIndex);
				}
				nodeMeshIndices.push_back(aux);
			}

			TimerElapsed(__FUNCTION__);
		}
		void UpdateDirichletIndices(BoundaryConditions& dirichlets, DofIndex& pivot, DofMeshIndices& dofMeshIndices, DirichletMeshIndices& dirichletMeshIndices)
		{
			TimerStart();

			DirichletMeshIndex dirichletMeshIndex;

			bool flag = false;

			pivot = 0;
			dirichletMeshIndices.clear();
			dirichletMeshIndices.reserve(dirichlets.size());

			for (Index i = 0; i < dirichlets.size(); ++i)
			{
				dirichletMeshIndex.node = dirichlets[i]->GetNode();
				dirichletMeshIndex.dofIndex = dirichlets[i]->GetDofIndex();

				auto dofIndex = dirichletMeshIndex.dofIndex;

				auto dofMeshIndex = std::lower_bound(dofMeshIndices.begin(), dofMeshIndices.end(), dirichletMeshIndex.node,
					[&](DofMeshIndex dofMeshIndex, INodePtr node) -> bool
					{
						if (dofMeshIndex.node != node)
						{
							return dofMeshIndex.node->GetTag() < node->GetTag();
						}

						return dofMeshIndex.dofIndex < dofIndex;
					});

				if (dofMeshIndex == dofMeshIndices.end())
				{
					logger::Warning(headerDive, "Dirichlet load applied to an invalid node. Ignored");
					continue;
				}

				if (dofMeshIndex->dirichlet != nullptr)
				{
					if (!flag)
					{
						logger::Warning(headerDive, "More than one dirichlet boundary condition applied to same dof");
						flag = true;
					}

					continue;
				}

				dofMeshIndex->dirichlet = dirichlets[i];
				dofMeshIndex->dirichletIndex = i;

				dirichletMeshIndices.push_back(dirichletMeshIndex);
			}

			for (auto& dofMeshIndex: dofMeshIndices)
			{
				if (dofMeshIndex.dirichlet != nullptr)
				{
					pivot++;
				}
			}

			TimerElapsed(__FUNCTION__);
		}
		void UpdateDirichletLoads(const DofMeshIndices& dofMeshIndices)
		{
			TimerStart();

			for (auto& dofMeshIndex : dofMeshIndices)
			{
				if (dofMeshIndex.dirichlet != nullptr)
				{
					auto node = dofMeshIndex.dirichlet->GetNode();
					auto dofIndex = dofMeshIndex.dirichlet->GetDofIndex();
					auto value = dofMeshIndex.dirichlet->GetValue();
	
					node->SetValue(dofIndex, value);
				}
			}

			TimerElapsed(__FUNCTION__);
		}
		void Reorder(IMeshPtr mesh, NumberDof numberDof, NodeMeshIndices& nodeMeshIndices, DofMeshIndices& dofMeshIndices)
		{
			TimerStart();

			Index counter{ 0 };
			
			for (Index i = 0; i < dofMeshIndices.size(); ++i)
			{
				if (dofMeshIndices[i].dirichlet != nullptr)
				{
					if (dofMeshIndices[i].dirichletReordered == false)
					{
						std::swap(dofMeshIndices[i].globalIndex, dofMeshIndices[counter].globalIndex);
						std::swap(dofMeshIndices[i], dofMeshIndices[counter]);

						dofMeshIndices[counter].dirichletReordered = true;

						counter++;
					}
				}
			}

			SortDofMeshIndices(dofMeshIndices);

			for (Index i = 0; i < dofMeshIndices.size(); ++i)
			{
				auto dofIndex = dofMeshIndices[i].dofIndex;
				auto globalIndex = dofMeshIndices[i].globalIndex;

				dofMeshIndices[i].node->GetConnectivity().globalDofIndices[dofIndex] = globalIndex;
			}

			const auto& elements = mesh->GetElements();

			for (ElementIndex i = 0; i < elements.size(); ++i)
			{
				for (NodeIndex j = 0; j < elements[i]->GetNumberNodes(); ++j)
				{
					auto nodeTag = elements[i]->GetNode(j)->GetTag();

					auto it = std::lower_bound(dofMeshIndices.begin(), dofMeshIndices.end(), nodeTag,
						[&](DofMeshIndex dofMeshIndex, Tag value) -> bool
						{
							return dofMeshIndex.node->GetTag() < value;
						});

					if (it != dofMeshIndices.end())
					{
						for (DofIndex k = 0; k < numberDof; ++k)
						{
							nodeMeshIndices[i][j].dofIndices[k] = (it + k)->globalIndex;
						}
					}
				}
			}

			TimerElapsed(__FUNCTION__);
		}
		void SortDofMeshIndices(DofMeshIndices& dofMeshIndices)
		{
			TimerStart();

			std::sort(dofMeshIndices.begin(), dofMeshIndices.end(),
				[&](DofMeshIndex dofMeshIndex1, DofMeshIndex dofMeshIndex2) -> bool
				{
					if (dofMeshIndex1.node != dofMeshIndex2.node)
					{
						return dofMeshIndex1.node->GetTag() < dofMeshIndex2.node->GetTag();
					}

					return dofMeshIndex1.dofIndex < dofMeshIndex2.dofIndex;
				});

			TimerElapsed(__FUNCTION__);
		}
	} // namespace problems
} // namespace dive