#include<vector>
template<typename T>
struct Scalar {
	T val;
	friend Scalar<T> operator+(Scalar<T> & rhs, Scalar<T> & lhs)
	{
		Scalar<T> res;
		res.val = rhs.val + lhs.val;
		return res;
	}
	Scalar<T> &  operator=(Scalar<T> & lhs)
	{
		val = lhs.val;
		return *this;
	}
};
struct Vec2
{
	double x = 0.0;
	double y = 0.0;
	Vec2() {}
	Vec2(int p_x, int p_y)
	{
		x = p_x;
		y = p_y;
	}
	friend Vec2 operator+(Vec2 & rhs, Vec2 & lhs)
	{
		Vec2 res;
		res.x = rhs.x + lhs.x;
		res.y = rhs.y + lhs.y;
		return res;
	}
};

std::vector<std::vector<Vec2>> v0;//prev
std::vector<std::vector<Vec2>> v1;
std::vector<std::vector<Scalar<double>>> s0;//prev
std::vector<std::vector<Scalar<double>>> s1;
std::vector<std::vector<Scalar<double>>> sources;
int width = 100;
double diff = 0.001;
double delT = 0.1;
double viscosity = 0.001;

void AdvectDensity();
void AdvectVelocity();
void InitializeField();
void SetVelocityBoundary();
void SetDensityBoundary();
void LinearVelocitySolver();
void LinearDensitySolver();

void InitializeField()
{
	s0.resize(width + 2);
	s1.resize(width + 2);
	v0.resize(width + 2);
	v1.resize(width + 2);
	sources.resize(width + 2);
	for (int i = 0; i < width + 2; i++)
	{
		s0[i].resize(width + 2);
		s1[i].resize(width + 2);
		v0[i].resize(width + 2, Vec2(0, 0));
		v1[i].resize(width + 2, Vec2(0, 0));
		sources[i].resize(width + 2);
	}

}
void DiffuseVelocity()
{
	LinearVelocitySolver();
}
void DiffuseDensity()
{
	LinearDensitySolver();
}
void SetDensityBoundary()
{
	for (int i = 1; i <= width; i++)
	{
		s1[0][i].val = s1[1][i].val;
		s1[width+1][width + 1].val = s1[width][width + 1].val;
		s1[i][0].val = s1[i][1].val;
		s1[i][width+1].val = s1[i][width].val;
		
	}
}
void SetVelocityBoundary()
{
	for (int i = 1; i <= width; i++)
	{
		v1[0][i].x = v1[1][i].x;
		v1[width + 1][i].x = v1[width][i].x;
		v1[0][i].y = v1[1][i].y;
		v1[width + 1][i].y = v1[width][i].y;
	}
}

void LinearDensitySolver()
{
	double a = delT * diff * width * width;
	double c = 1.0 + 4 * a;
	for (int k = 0; k < 20; k++)
	{
		for (int i = 1; i <= width; i++)
		{
			for (int j = 1; j <= width; j++)
			{
				s1[i][j].val = (s0[i][j].val + a * (s1[i - 1][j].val + s1[i + 1][j].val + s1[i][j - 1].val + s1[i][j + 1].val)) / c;
			}
		}
		SetDensityBoundary();
	}
}
void LinearVelocitySolver()
{
	double a = delT * viscosity * width * width;
	double c = 1.0 + 4 * a;
	for (int k = 0; k < 20; k++)
	{
		for (int i = 1; i <= width; i++)
		{
			for (int j = 1; j <= width; j++)
			{
				v1[i][j].x = (v0[i][j].x + a * (v1[i - 1][j].x + v1[i + 1][j].x + v1[i][j - 1].x + v1[i][j + 1].x)) / c;
				v1[i][j].y = (v0[i][j].y + a * (v1[i - 1][j].y + v1[i + 1][j].y + v1[i][j - 1].y + v1[i][j + 1].y)) / c;
			}
		}
		SetDensityBoundary();
	}
}
void AdvectDensity()
{
	double x, y, k0, t0, k1, t1, dt0;
	int i0, j0, i1, j1;
	dt0 = delT * width;
	for (int i = 1; i <= width; i++)
	{
		for (int j = 1; j <= width; j++)
		{
			x = i - dt0 * v1[i][j].x;
			y = j - dt0 * v1[i][j].y;
			if (x < 0.5) x = 0.5;
			if (x > width + 0.5) x = width + 0.5;
			if (y < 0.5) y = 0.5;
			if (y > width + 0.5) y = width + 0.5;
			i0 = (int)x;
			j0 = (int)y;
			i1 = i0 + 1;
			j1 = j0 + 1;
			k1 = x - i0;
			k0 = 1 - k1;
			t1 = y - j0;
			t0 = 1 - t1;
			s1[i][j].val = k0 * (t0*s0[i0][j0].val + t1 * s0[i0][j1].val) + k1 * (t0*s0[i1][j0].val + t1 * s0[i1][j1].val);
		}
	}
	SetDensityBoundary();

}
void AdvectVelocity()
{
	double x, y, k0, t0, k1, t1, dt0;
	int i0, j0, i1, j1;
	dt0 = delT * width;
	for (int i = 1; i <= width; i++)
	{
		for (int j = 1; j <= width; j++)
		{
			x = i - dt0 * v0[i][j].x;
			y = j - dt0 * v0[i][j].y;
			if (x < 0.5)         x = 0.5;
			if (x > width + 0.5) x = width + 0.5;
			if (y < 0.5)         y = 0.5;
			if (y > width + 0.5) y = width + 0.5;
			i0 = (int)x;
			j0 = (int)y;
			i1 = i0 + 1;
			j1 = j0 + 1;
			k1 = x - i0;
			k0 = 1 - k1;
			t1 = y - j0;
			t0 = 1 - t1;
			v1[i][j].x = k0 * (t0*v0[i0][j0].x + t1 * v0[i0][j1].x) + k1 * (t0*v0[i1][j0].x + t1 * v0[i1][j1].x);
			v1[i][j].y = k0 * (t0*v0[i0][j0].y + t1 * v0[i0][j1].y) + k1 * (t0*v0[i1][j0].y + t1 * v0[i1][j1].y);
		}
	}
	SetVelocityBoundary();

}

void ScalarBoundary()
{
	for (int i = 1; i <= width; i++)
	{
		v0[i][0] = v0[i][1];
		v0[i][width + 1] = v0[i][width];
		v0[0][i] = v0[1][i];
		v0[width + 1][i] = v0[width][i];

	}
}
void ProjectVelocityField()
{
	double dX = 1 / double(width);
	for (int i = 1; i <= width; i++)
	{
		for (int j = 1; j <= width; j++) {
			v0[i][j].x = -0.5 * dX * ((v1[i + 1][j].x - v1[i - 1][j].x) + (v1[i][j + 1].y - v1[i][j - 1].y));
			v0[i][j].y = 0;
		}
	}
	ScalarBoundary();
	for (int k = 0; k < 20; k++)
	{
		for (int i = 1; i <= width; i++)
		{
			for (int j = 1; j <= width; j++)
			{
				v0[i][j].y = (v0[i][j].x + v0[i + 1][j].y + v0[i - 1][j].y + v0[i][j + 1].y + v0[i][j - 1].y) / 4;
			}
		}
		ScalarBoundary();
	}
	for (int i = 1; i <= width; i++)
	{
		for (int j = 1; j <= width; j++)
		{
			v1[i][j].x -= 0.5 * (v0[i + 1][j].y - v0[i - 1][j].y) / dX;
			v1[i][j].y -= 0.5 * (v0[i][j + 1].y - v0[i][j - 1].y) / dX;
		}
	}
	SetVelocityBoundary();
}
void Vel_Step()
{
	v0.swap(v1);
	DiffuseVelocity();
	ProjectVelocityField();
	v0.swap(v1);
	AdvectVelocity();
	ProjectVelocityField();

}
void Dens_Step()
{
	s0.swap(s1);
	DiffuseDensity();
	s0.swap(s1);
	AdvectDensity();

}
