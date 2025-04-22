#include "dive_weakforms_basic.hpp"
#include "dive_loads.hpp"
#include "dive_loads_distributed_volume.hpp"
#include "dive_loads_distributed_face.hpp"
#include "dive_loads_distributed_edge.hpp"

namespace dive {
	namespace weakforms {		
		void WeakFormElement::IntegrationVolume(IElementPtr element, Matrix& output) const
		{
			Matrix local;

			const auto& gauss = element->IntegralVolume();
			const auto& points = gauss->GetPoints();
			const auto& weights = gauss->GetWeights();
			const auto& counter = gauss->GetCounter();

			WeakFormulation(element, 0, points[0], local);
	
			output = weights[0] * element->DetJ(points[0], 0) * local;

			for (quadrature::Counter i = 1; i < counter; ++i)
			{
				WeakFormulation(element, i, points[i], local);

				output = output + weights[i] * element->DetJ(points[i], i) * local;
			}
		}

		void WeakFormLoad::IntegrationVolume(ILoadPtr load, Vector& output) const
		{
			auto force = std::static_pointer_cast<loads::LoadDistributedVolume>(load);
			auto element = force->GetElement();
			Vector local;

			const auto& gauss = element->IntegralVolume();
			const auto& points = gauss->GetPoints();
			const auto& weights = gauss->GetWeights();
			const auto& counter = gauss->GetCounter();

			WeakFormulation(load, points[0], local);
			output = weights[0] * element->DetJ(points[0], 0) * local;

			for (quadrature::Counter i = 1; i < counter; ++i)
			{
				WeakFormulation(load, points[i], local);

				output = output + weights[i] * element->DetJ(points[i], i) * local;
			}
		}
		void WeakFormLoad::IntegrationFace(ILoadPtr load, Vector& output) const
		{
			auto force = std::static_pointer_cast<loads::LoadDistributedFace>(load);
			auto element = force->GetElement();
			Vector local;

			Vector point(element->GetDimension());

			const auto& gauss = element->IntegralArea();
			const auto& points = gauss->GetPoints();
			const auto& weights = gauss->GetWeights();
			const auto& counter = gauss->GetCounter();

			auto faceIndex = force->GetFaceIndex();
			auto helper = element->GetIntegralAreaHelper(faceIndex);

			point(helper.dim1) = points[0](0);
			point(helper.dim2) = points[0](1);
			point(helper.dim3) = helper.coord3;

			WeakFormulation(load, point, local);
			output = output + weights[0] * element->DelA(point, helper.dim1, helper.dim2) * local;

			for (quadrature::Counter i = 1; i < counter; ++i)
			{
				point(helper.dim1) = points[i](0);
				point(helper.dim2) = points[i](1);
				point(helper.dim3) = helper.coord3;

				WeakFormulation(load, point, local);

				output = output + weights[i] * element->DelA(point, helper.dim1, helper.dim2) * local;
			}
		}
		void WeakFormLoad::IntegrationEdge(ILoadPtr load, Vector& output) const
		{
			auto force = std::static_pointer_cast<loads::LoadDistributedEdge>(load);
			auto element = force->GetElement();
			Vector local;
			Index counter = 0;

			Vector point(element->GetDimension());

			const auto& gauss = element->IntegralEdge();
			const auto& points = gauss->GetPoints();
			const auto& weights = gauss->GetWeights();
			const auto& quadrature = gauss->GetQuadrature();

			auto edgeIndex = force->GetEdgeIndex();
			auto helper = element->GetIntegralEdgeHelper(edgeIndex);

			point(helper.dim1) = points[0](0);
			point(helper.dim2) = helper.coord2;
			point(helper.dim3) = helper.coord3;

			WeakFormulation(load, point, local);
			output = output + weights[0] * element->DelL(point, helper.dim1) * local;

			for (quadrature::Counter i = 1; i < counter; ++i)
			{
				point(helper.dim1) = points[counter](0);
				point(helper.dim2) = helper.coord2;
				point(helper.dim3) = helper.coord3;

				WeakFormulation(load, point, local);

				output = output + weights[counter] * element->DelL(point, helper.dim1) * local;

				++counter;
			}
		}
	} // namespace weakforms
} // namespace dive