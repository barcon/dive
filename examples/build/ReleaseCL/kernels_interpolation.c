#define EILIG_SCALAR double
#define EILIG_SIZE_T long unsigned int

struct Point
{
	EILIG_SCALAR x;
	EILIG_SCALAR y;
	EILIG_SCALAR z;
};

bool IsAlmostEqual(EILIG_SCALAR x, EILIG_SCALAR y, int ulp)
{	
	return (fabs(x - y) <= DBL_EPSILON * fabs(x + y) * ulp) || (fabs(x - y) < DBL_MIN);
}

EILIG_SCALAR Distance(struct Point point1, struct Point point2)
{
	EILIG_SCALAR dx = point1.x - point2.x;
	EILIG_SCALAR dy = point1.y - point2.y;
	EILIG_SCALAR dz = point1.z - point2.z;

	return sqrt(dx * dx + dy * dy + dz * dz);
}

EILIG_SCALAR FunctionTPS(struct Point point1, struct Point point2) 
{
	EILIG_SCALAR r = Distance(point1, point2);

	return IsAlmostEqual(r, 0.0, 2) ? 0.0 : r * r * log(r);
}
EILIG_SCALAR FunctionMQB(struct Point point1, struct Point point2, EILIG_SCALAR shape) 
{
	EILIG_SCALAR r = Distance(point1, point2);

	return sqrt(shape * shape + r * r);
}
EILIG_SCALAR FunctionGAU(struct Point point1, struct Point point2, EILIG_SCALAR shape) 
{
	EILIG_SCALAR r = Distance(point1, point2);

	return exp(-(shape * r) * (shape * r));
}
EILIG_SCALAR FunctionINQ(struct Point point1, struct Point point2, EILIG_SCALAR shape) 
{
	EILIG_SCALAR r = Distance(point1, point2);

	return 1.0 / (r * r + shape * shape);
}

__kernel void FunctionTPS1(const EILIG_SIZE_T rows, const EILIG_SIZE_T cols, __global EILIG_SCALAR* points, __global EILIG_SCALAR* data)
{
	size_t i = get_global_id(0);
	size_t numberCoordinates = 3; // Assuming 3D points (x, y, z)

	if(i < rows)
	{
		struct Point point1 = { points[i * numberCoordinates + 0], points[i * numberCoordinates + 1], points[i * numberCoordinates + 2] };
		
		for(size_t j = 0; j < cols; j++)
		{
			struct Point point2 = { points[j * numberCoordinates + 0], points[j * numberCoordinates + 1], points[j * numberCoordinates + 2] };

			data[i * cols + j] = FunctionTPS(point1, point2);
		}
	}
};
__kernel void FunctionMQB1(const EILIG_SIZE_T rows, const EILIG_SIZE_T cols, __global EILIG_SCALAR* points, __global EILIG_SCALAR* data, const EILIG_SCALAR shape)
{
	size_t i = get_global_id(0);
	size_t numberCoordinates = 3; // Assuming 3D points (x, y, z)

	if (i < rows)
	{
		struct Point point1 = { points[i * numberCoordinates + 0], points[i * numberCoordinates + 1], points[i * numberCoordinates + 2] };

		for (size_t j = 0; j < cols; j++)
		{
			struct Point point2 = { points[j * numberCoordinates + 0], points[j * numberCoordinates + 1], points[j * numberCoordinates + 2] };

			data[i * cols + j] = FunctionMQB(point1, point2, shape);
		}
	}
};
__kernel void FunctionGAU1(const EILIG_SIZE_T rows, const EILIG_SIZE_T cols, __global EILIG_SCALAR* points, __global EILIG_SCALAR* data, const EILIG_SCALAR shape)
{
	size_t i = get_global_id(0);
	size_t numberCoordinates = 3; // Assuming 3D points (x, y, z)

	if (i < rows)
	{
		struct Point point1 = { points[i * numberCoordinates + 0], points[i * numberCoordinates + 1], points[i * numberCoordinates + 2] };

		for (size_t j = 0; j < cols; j++)
		{
			struct Point point2 = { points[j * numberCoordinates + 0], points[j * numberCoordinates + 1], points[j * numberCoordinates + 2] };

			data[i * cols + j] = FunctionGAU(point1, point2, shape);
		}
	}
};
__kernel void FunctionINQ1(const EILIG_SIZE_T rows, const EILIG_SIZE_T cols, __global EILIG_SCALAR* points, __global EILIG_SCALAR* data, const EILIG_SCALAR shape)
{
	size_t i = get_global_id(0);
	size_t numberCoordinates = 3; // Assuming 3D points (x, y, z)

	if (i < rows)
	{
		struct Point point1 = { points[i * numberCoordinates + 0], points[i * numberCoordinates + 1], points[i * numberCoordinates + 2] };

		for (size_t j = 0; j < cols; j++)
		{
			struct Point point2 = { points[j * numberCoordinates + 0], points[j * numberCoordinates + 1], points[j * numberCoordinates + 2] };

			data[i * cols + j] = FunctionINQ(point1, point2, shape);
		}
	}
};

__kernel void FunctionTPS2(const EILIG_SIZE_T numberPoints, const EILIG_SIZE_T numberNodes, __global EILIG_SCALAR* points, __global EILIG_SCALAR* nodes, __global EILIG_SCALAR* data)
{
	size_t i = get_global_id(0);
	size_t numberCoordinates = 3; // Assuming 3D points (x, y, z)

	if (i < numberPoints)
	{
		struct Point point1 = { points[i * numberCoordinates + 0], points[i * numberCoordinates + 1], points[i * numberCoordinates + 2] };

		for (size_t j = 0; j < numberNodes; j++)
		{
			struct Point point2 = { nodes[j * numberCoordinates + 0], nodes[j * numberCoordinates + 1], nodes[j * numberCoordinates + 2] };

			data[i * numberNodes + j] = FunctionTPS(point1, point2);
		}
	}
};
__kernel void FunctionMQB2(const EILIG_SIZE_T numberPoints, const EILIG_SIZE_T numberNodes, __global EILIG_SCALAR* points, __global EILIG_SCALAR* nodes, __global EILIG_SCALAR* data, const EILIG_SCALAR shape)
{
	size_t i = get_global_id(0);
	size_t numberCoordinates = 3; // Assuming 3D points (x, y, z)

	if (i < numberPoints)
	{
		struct Point point1 = { points[i * numberCoordinates + 0], points[i * numberCoordinates + 1], points[i * numberCoordinates + 2] };

		for (size_t j = 0; j < numberNodes; j++)
		{
			struct Point point2 = { nodes[j * numberCoordinates + 0], nodes[j * numberCoordinates + 1], nodes[j * numberCoordinates + 2] };

			data[i * numberNodes + j] = FunctionMQB(point1, point2, shape);
		}
	}
};
__kernel void FunctionGAU2(const EILIG_SIZE_T numberPoints, const EILIG_SIZE_T numberNodes, __global EILIG_SCALAR* points, __global EILIG_SCALAR* nodes, __global EILIG_SCALAR* data, const EILIG_SCALAR shape)
{
	size_t i = get_global_id(0);
	size_t numberCoordinates = 3; // Assuming 3D points (x, y, z)

	if (i < numberPoints)
	{
		struct Point point1 = { points[i * numberCoordinates + 0], points[i * numberCoordinates + 1], points[i * numberCoordinates + 2] };

		for (size_t j = 0; j < numberNodes; j++)
		{
			struct Point point2 = { nodes[j * numberCoordinates + 0], nodes[j * numberCoordinates + 1], nodes[j * numberCoordinates + 2] };

			data[i * numberNodes + j] = FunctionGAU(point1, point2, shape);
		}
	}
};
__kernel void FunctionINQ2(const EILIG_SIZE_T numberPoints, const EILIG_SIZE_T numberNodes, __global EILIG_SCALAR* points, __global EILIG_SCALAR* nodes, __global EILIG_SCALAR* data, const EILIG_SCALAR shape)
{
	size_t i = get_global_id(0);
	size_t numberCoordinates = 3; // Assuming 3D points (x, y, z)

	if (i < numberPoints)
	{
		struct Point point1 = { points[i * numberCoordinates + 0], points[i * numberCoordinates + 1], points[i * numberCoordinates + 2] };

		for (size_t j = 0; j < numberNodes; j++)
		{
			struct Point point2 = { nodes[j * numberCoordinates + 0], nodes[j * numberCoordinates + 1], nodes[j * numberCoordinates + 2] };

			data[i * numberNodes + j] = FunctionINQ(point1, point2, shape);
		}
	}
};

__kernel void DotProduct(const EILIG_SIZE_T numberNodes, const EILIG_SIZE_T numberPoints, const EILIG_SIZE_T numberDof, __global EILIG_SCALAR* points, __global EILIG_SCALAR* rbf, __global EILIG_SCALAR* alpha, __global EILIG_SCALAR* data)
{
	size_t i = get_global_id(0);

	if (i < numberPoints)
	{
		for (size_t dof = 0; dof < numberDof; dof++)
		{
			EILIG_SCALAR sum = 0.0;
			for (size_t j = 0; j < numberNodes; j++)
			{
				sum += rbf[i * numberNodes + j] * alpha[j * numberDof + dof];
			}

			data[i * numberPoints + dof] = sum;
		}
	}
};