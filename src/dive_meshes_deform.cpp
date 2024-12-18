#include "dive_meshes_deform.hpp"
#include "dive_meshes.hpp"

namespace dive {
	namespace meshes {

		void DeformByInterpolation(MeshPtr mesh, InterpolationPtr interpolation)
		{
			std::size_t rows;
			Vector point;
			Vector disp;
			Matrix mat;
			NodePtr aux{ nullptr };
			
			auto& nodes = mesh->GetNodes();

			for (auto& node: nodes)
			{
				point = node->GetPoint();	

				aux = nodes::Create();
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