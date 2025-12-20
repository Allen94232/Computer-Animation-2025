#include "fluid.h"
#include <iostream>
void Fluid::integrate()
{
	Eigen::Vector2f gravityVec(0, -gravity * dt);

	for (int i = 0; i < num_particles; ++i)
	{
		particle_vel[i] += gravityVec;
		particle_pos[i] += dt * particle_vel[i];
	}
}

void Fluid::handleCollisions(bool lmbDown)
{
	Eigen::Vector2f minVal(cell_dim + particle_radius, cell_dim + particle_radius),
		maxVal(viewport_w - cell_dim - particle_radius, viewport_h - cell_dim - particle_radius);
	
	float min_obstacle_d2 = (obstacle_r + particle_radius) * (obstacle_r + particle_radius);
	Eigen::Vector2f obstacle_pos(obstacle_x, obstacle_y), obstacle_vel(obstacle_vx, obstacle_vy);

	for (int i = 0; i < num_particles; ++i)
	{
		Eigen::Vector2f& pos = particle_pos[i];
		Eigen::Vector2f& vel = particle_vel[i];

		// collision with obstacle
		if (lmbDown)
		{
			float d2_to_obstacle = (pos - obstacle_pos).squaredNorm();
			if (d2_to_obstacle < min_obstacle_d2)
				vel = obstacle_vel;
		}

		// collision with edges
		Eigen::Vector2f clamped = pos.cwiseMax(minVal).cwiseMin(maxVal);

		if (clamped.x() != pos.x()) vel.x() = 0.0f;
		if (clamped.y() != pos.y()) vel.y() = 0.0f;

		pos = clamped;
	}
}

void Fluid::particleRelaxation()
{
	/*TODO*/
	/*
	This function handles inter-particle collisions, here's what you'll have to implement:

	1.	Load all particles indices into relaxation cells

	2.	Implement the iterative solver:

		At each iteration, go through all relaxation cells.
		For each particle in the cell, compare its position to every other particle within a 3x3 region.
		If two particles are colliding, i.e. |d| < 2 * r, shift them away from each other so the two particles are 2 * r units apart.
		[DEBUGGING HINT: Two particles may completely overlap with each other!]

		You can directly apply the displacement vectors right after their calculation.


	Some of the variables you might need, but not limited to:

	Relaxation Cells:
		relaxation_cell_col				// Number of relaxation cell columns
		relaxation_cell_rows			// Number of relaxation cell rows
		relaxation_cell_particle_ids	// Index of particles in each cell
		relaxation_cell_dim				// Dimension of each relaxation cell (same width and height)


	Particle Information:
		particle_pos					// Position (x, y) of each particle
		particle_radius	
		num_particles
	*/

	// TODO: Assign particles to cells
	for (int i = 0; i < relaxation_cell_rows; ++i)
		for (int j = 0; j < relaxation_cell_cols; ++j)
			relaxation_cell_particle_ids[i][j].clear();

	for (int i = 0; i < num_particles; ++i)
	{
		int cell_x = static_cast<int>(particle_pos[i].x() / relaxation_cell_dim);
		int cell_y = static_cast<int>(particle_pos[i].y() / relaxation_cell_dim);
		if (cell_x >= 0 && cell_x < relaxation_cell_cols && cell_y >= 0 && cell_y < relaxation_cell_rows)
			relaxation_cell_particle_ids[cell_y][cell_x].push_back(i);
	}
		
	for (int iter = 0; iter < iterations; ++iter)
	{
		for (int i = 0; i < relaxation_cell_rows; ++i) for (int j = 0; j < relaxation_cell_cols; ++j)
		{
			// TODO: Perform particle relaxation
			for (int k = 0; k < relaxation_cell_particle_ids[i][j].size(); ++k)
			{
				int id1 = relaxation_cell_particle_ids[i][j][k];
				Eigen::Vector2f& pos1 = particle_pos[id1];

				// Search neighboring cells
				for (int di = -1; di <= 1; ++di)
				{
					for (int dj = -1; dj <= 1; ++dj)
					{
						int ni = i + di;
						int nj = j + dj;
						if (ni < 0 || ni >= relaxation_cell_rows || nj < 0 || nj >= relaxation_cell_cols)
							continue;

						for (int l = 0; l < relaxation_cell_particle_ids[ni][nj].size(); ++l)
						{
							int id2 = relaxation_cell_particle_ids[ni][nj][l];
							if (id1 >= id2) continue;

							Eigen::Vector2f& pos2 = particle_pos[id2];
							Eigen::Vector2f d = pos1 - pos2;
							float d2 = d.squaredNorm();

							if (d2 < 1e-8f)
							{
								d = Eigen::Vector2f::Random().normalized() * 1e-4f;
								d2 = d.squaredNorm();
							}

							if (d2 < 4 * particle_radius * particle_radius)
							{
								float dist = sqrt(d2);
								float overlap = 2 * particle_radius - dist;
								Eigen::Vector2f correction = 0.5f * overlap * d / dist;
								pos1 += correction;
								pos2 -= correction;
							}
						}
					}
				}
			}
		}
	}
}

void Fluid::transferVelocities(bool to_cell)
{
	/*TODO*/
	/*
	This function has a parameter to_cell that determines whether the particle velocities should be transfered to the cells
	or in the other direction.

	1.	Some setup you need to do when to_cell is true:

		Reset all cell_velocities.
		Assign each cell their proper cell type. A cell should be assigned the FLUID type if it contains any particles and AIR if not.
		SOLID cells are present on initialization and should not be modified.

	2.	In main loop:

		For each of x and y components, calculate the 4 cells that should contribute to the biliear interpolation of each particle.
		From this step, we can obtain 4 weights, w_1 ~ w_4. These weights will be used in both scenarios (to_cell = true or false).

		When transfering particle velocities to the cells, add w_n * v to each of the 4 cell velocity components.

		We should also accumulate w_n of each sampling point in a separate buffer to normalize the cell velocities after
		iterating through all the particles.

		When transfering cell velocities back to the particles, before doing the usual bilinear interpolation, we need to test if
		the velocities are valid.

		Let's take a look at an example, where we calculate the x velocity component

				|		|
		-------------------------
				|		|			vx_1: Vx(i, j)
			F	•->	A	•->	A		vx_2: Vx(i + 1, j)
		   Vx_4	|   	| Vx_3		vx_3: Vx(i + 1, j + 1)
		-------------------------	vx_4: Vx(i, j + 1)
				|	  ⦾	|
			F	•->	F <-•	A		F: Fluid Cells
		   Vx_1	|		| Vx_2		A: Air Cells
		-------------------------
				|		|

		In this case, only vx_3 is invalid since the value it carries is derived from 2 AIR cells.
		Note, since the Vy is sampled at the center of the horizontal edge, we should test the upper and lower cells in that case.

		After this we can finally obtain the interpolated velocity. In the provided example, since Vx_3 is invalid, the interpolated velocity
		will be:

					w_1 * vx_1 + w_2 * vx_2 + w_4 * vx_4
		 Vx_pic =	------------------------------------
							  w_1 + w_2 + w_4

		We're almost done!
		The particle velocity was calculated using the PIC method. To obtain FLIP velocity use this formula:

							w_1 * (vx_1 - prev_v1) + w_2 * (vx_2 - prev_v2) + w_4 * (vx_4 - prev_v4)
		 Vx_flip =	Vx_p +	-----------------------------------------------------------------------
													   w_1 + w_2 + w_4

		We can finally set the particle velocity using a blend of PIC and FLIP velocities:

		 Vx_p = (1 - flip_ratio) * Vx_pic + flip_ratio * Vx_flip

		Don't forget to do this in the Y direction as well!

	3.	After the loop, if we're transfering cell velocities to the particle, we'll have to normalize each velocity component using
		the stored normalizing values mentioned before.

		Backup cell velocities in prev_cell_velocities for FLIP calculation.


	Some of the variables you might need, but not limited to:

	flip_ratio					# used during pic/flip interpolation

	Particle Information:
		particle_pos
		particle_vel
		num_particles

	MAC Cells:
		cell_rows
		cell_cols
		cell_dim
		cell_types				# Type of each cell. {CellType::FLUID, CellType::AIR, CellType::SOLID}
		cell_velocities			# Vx (sampled at the left edge of the cell)
								# Vy (sampled at the bottom edge of the cell)

		prev_cell_velocities

	A 2d vector to store the normalizing terms for cell velicities when to_cell is true

	*/


	if (to_cell)
	{
		// TODO: Reset velocities and update cell types
		for (int i = 0; i < cell_rows; ++i)
		{
			for (int j = 0; j < cell_cols; ++j)
			{
				cell_velocities[i][j] = Eigen::Vector2f::Zero();
				prev_cell_velocities[i][j] = Eigen::Vector2f::Zero();

				if (cell_types[i][j] != CellType::SOLID)
					cell_types[i][j] = CellType::AIR;
			}
		}

		for (int i = 0; i < num_particles; ++i)
		{
			int cx = static_cast<int>(particle_pos[i].x() / cell_dim);
			int cy = static_cast<int>(particle_pos[i].y() / cell_dim);

			if (cx >= 0 && cx < cell_cols && cy >= 0 && cy < cell_rows && cell_types[cy][cx] != CellType::SOLID)
				cell_types[cy][cx] = CellType::FLUID;
		}
	}

	for (int component = 0; component < 2; ++component)
	{
		std::vector<std::vector<float>> normalizing_terms(cell_rows, std::vector<float>(cell_cols, 0.0f));

		if (to_cell)
		{
			for (int i = 0; i < num_particles; ++i)
			{
				Eigen::Vector2f& pos = particle_pos[i];
				Eigen::Vector2f& vel = particle_vel[i];

				float px = pos.x();
				float py = pos.y();

				if (component == 0) px -= 0.5f * cell_dim;  // for u-grid
				else               py -= 0.5f * cell_dim;  // for v-grid

				float fx = px / cell_dim;
				float fy = py / cell_dim;
				int cx = static_cast<int>(fx);
				int cy = static_cast<int>(fy);

				if (cx < 0 || cx + 1 >= cell_cols || cy < 0 || cy + 1 >= cell_rows)
					continue;

				float dx = fx - cx;
				float dy = fy - cy;

				float w1 = (1 - dx) * (1 - dy);
				float w2 = dx * (1 - dy);
				float w3 = dx * dy;
				float w4 = (1 - dx) * dy;

				float v = (component == 0) ? vel.x() : vel.y();

				if (component == 0)
				{
					cell_velocities[cy][cx].x() += w1 * v;
					cell_velocities[cy][cx + 1].x() += w2 * v;
					cell_velocities[cy + 1][cx + 1].x() += w3 * v;
					cell_velocities[cy + 1][cx].x() += w4 * v;
				}
				else
				{
					cell_velocities[cy][cx].y() += w1 * v;
					cell_velocities[cy][cx + 1].y() += w2 * v;
					cell_velocities[cy + 1][cx + 1].y() += w3 * v;
					cell_velocities[cy + 1][cx].y() += w4 * v;
				}

				normalizing_terms[cy][cx] += w1;
				normalizing_terms[cy][cx + 1] += w2;
				normalizing_terms[cy + 1][cx + 1] += w3;
				normalizing_terms[cy + 1][cx] += w4;
			}

			for (int i = 0; i < cell_rows; ++i)
			{
				for (int j = 0; j < cell_cols; ++j)
				{
					if (normalizing_terms[i][j] > 1e-6f)
					{
						if (component == 0)
						{
							cell_velocities[i][j].x() /= normalizing_terms[i][j];
							prev_cell_velocities[i][j].x() = cell_velocities[i][j].x();
						}
						else
						{
							cell_velocities[i][j].y() /= normalizing_terms[i][j];
							prev_cell_velocities[i][j].y() = cell_velocities[i][j].y();
						}
					}
					else
					{
						if (component == 0) cell_velocities[i][j].x() = 0.0f;
						else cell_velocities[i][j].y() = 0.0f;
					}
				}
			}
		}
		else
		{
			for (int p = 0; p < num_particles; ++p)
			{
				Eigen::Vector2f pos = particle_pos[p];

				// Sample at staggered grid center
				if (component == 0) pos.x() -= 0.5f * cell_dim;
				else pos.y() -= 0.5f * cell_dim;

				float fx = pos.x() / cell_dim;
				float fy = pos.y() / cell_dim;
				int cx = static_cast<int>(fx);
				int cy = static_cast<int>(fy);

				if (cx < 0 || cy < 0 || cx + 1 >= cell_cols || cy + 1 >= cell_rows)
					continue;

				float dx = fx - cx;
				float dy = fy - cy;

				float w1 = (1 - dx) * (1 - dy);
				float w2 = dx * (1 - dy);
				float w3 = dx * dy;
				float w4 = (1 - dx) * dy;

				float pic = 0.0f;
				float flip = 0.0f;
				float total_weight = 0.0f;

				// 4-point bilinear interpolation with validity check
				for (int k = 0; k < 4; ++k)
				{
					int i = cy + (k == 2 || k == 3);
					int j = cx + (k == 1 || k == 2);
					if (i < 0 || j < 0 || i >= cell_rows || j >= cell_cols)
						continue;

					// For validity check: we require at least one adjacent FLUID cell
					bool valid = false;
					if (component == 0)
					{
						if (i > 0 && cell_types[i - 1][j] == CellType::FLUID) valid = true;
						if (i < cell_rows - 1 && cell_types[i][j] == CellType::FLUID) valid = true;
					}
					else
					{
						if (j > 0 && cell_types[i][j - 1] == CellType::FLUID) valid = true;
						if (j < cell_cols - 1 && cell_types[i][j] == CellType::FLUID) valid = true;
					}

					if (!valid) continue;

					float w = (k == 0) ? w1 : (k == 1) ? w2 : (k == 2) ? w3 : w4;
					float curr = (component == 0) ? cell_velocities[i][j].x() : cell_velocities[i][j].y();
					float prev = (component == 0) ? prev_cell_velocities[i][j].x() : prev_cell_velocities[i][j].y();

					pic += w * curr;
					flip += w * (curr - prev);
					total_weight += w;
				}

				if (total_weight > 1e-6f)
				{
					pic /= total_weight;
					flip /= total_weight;

					particle_vel[p][component] = (1.0f - flip_ratio) * pic + flip_ratio * (particle_vel[p][component] + flip);
				}
			}
		}
	}
}



void Fluid::updateDensity()
{
	/*TODO*/
	/*
	Here we will update the cell densities, which will be used to determine if a cell is overly compressed in the next step.
	The density of each cell is sampled at the center, and it uses the same concept as the transferVelocities function. 

	1.	Perform bilinear interpolation on each particle, each particle will contribute 1 * w_n to each of the sampling points.

	2.	Set the the resting density of water cells
		
		particle rest density = sum of all water cell densities / number of water cells

		This only needs to be done once, so simply test if particle_rest_density is 0 (the initial value).
		The particles are initialized to be tightly packed, so if you changed the layout of the particles, this step will have incorrect behavior.


	Some of the variables you might need, but not limited to:

	particle_rest_density		# updated only once at the first pass (test if particle_rest_density == 0)

	MAC cells:
		cell_densities
		cell_types
		cell_row
		cell_cols
		cell_dim

	Particle Information:
		num_particles
		particle_pos
	*/

	for (int i = 0; i < cell_rows; ++i) for (int j = 0; j < cell_cols; ++j)
		cell_densities[i][j] = 0.0f;

	
	for (int i = 0; i < num_particles; ++i)
	{
		// TODO: Perform bilinear interpolation
		const Eigen::Vector2f& pos = particle_pos[i];

		float fx = pos.x() / cell_dim;
		float fy = pos.y() / cell_dim;

		int cx = static_cast<int>(fx);
		int cy = static_cast<int>(fy);

		float dx = fx - cx;
		float dy = fy - cy;

		// Skip particles outside valid interpolation area
		if (cx < 0 || cx + 1 >= cell_cols || cy < 0 || cy + 1 >= cell_rows)
			continue;

		// Bilinear weights
		float w1 = (1 - dx) * (1 - dy);
		float w2 = dx * (1 - dy);
		float w3 = dx * dy;
		float w4 = (1 - dx) * dy;

		// Each particle contributes 1.0 * weight to nearby cells
		cell_densities[cy][cx] += w1;
		cell_densities[cy][cx + 1] += w2;
		cell_densities[cy + 1][cx + 1] += w3;
		cell_densities[cy + 1][cx] += w4;
	}

	if (particle_rest_density == 0.0) 
	{
		// TODO: Calculate resting particle densities in fluid cells.
		float total_density = 0.0f;
		int fluid_cell_count = 0;

		for (int i = 0; i < cell_rows; ++i)
			for (int j = 0; j < cell_cols; ++j)
			{
				if (cell_types[i][j] == FLUID)
				{
					total_density += cell_densities[i][j];
					fluid_cell_count++;
				}
			}

		if (fluid_cell_count > 0)
			particle_rest_density = total_density / fluid_cell_count;
		else
			particle_rest_density = 1.0f;  // Fallback to 1.0 if no FLUID cells are found
	}
}

void Fluid::solveIncompressibility()
{
	// Store velocities for FLIP
	for (int i = 0; i < cell_rows; ++i)
		for (int j = 0; j < cell_cols; ++j)
			prev_cell_velocities[i][j] = cell_velocities[i][j];

	for (int iter = 0; iter < iterations; ++iter)
	{
		for (int i = 1; i < cell_rows - 1; ++i)
		{
			for (int j = 1; j < cell_cols - 1; ++j)
			{
				// TODO: Calculate divergence of fluid cells
				if (cell_types[i][j] != FLUID)
					continue;

				float div = cell_velocities[i][j + 1].x() - cell_velocities[i][j].x()
						  + cell_velocities[i + 1][j].y() - cell_velocities[i][j].y();

				// TODO: Add bias to ouflow if density_correction is true
				if (density_correction)
					div -= stiffness_coefficient * (cell_densities[i][j] - particle_rest_density);

				int dirs = 0;
				if (cell_types[i][j - 1] != SOLID) dirs++;
				if (cell_types[i][j + 1] != SOLID) dirs++;
				if (cell_types[i - 1][j] != SOLID) dirs++;
				if (cell_types[i + 1][j] != SOLID) dirs++;

				if (dirs == 0)
					continue;

				float corr = over_relaxation * div / dirs;

				// Apply corrections (staggered grid)
				if (cell_types[i][j - 1] != SOLID)
					cell_velocities[i][j].x() += corr;

				if (cell_types[i][j + 1] != SOLID)
					cell_velocities[i][j + 1].x() -= corr;

				if (cell_types[i - 1][j] != SOLID)
					cell_velocities[i][j].y() += corr;

				if (cell_types[i + 1][j] != SOLID)
					cell_velocities[i + 1][j].y() -= corr;
			}
		}
	}
}

Fluid::Fluid(int num_particles, float radius, float obstacle_radius, float flip_ratio, float cell_dim, float relaxation_cell_dim, int iterations, float viewport_w, 
	float viewport_h, float dt, float gravity, float stiffness, bool density_correction, float over_relaxation)
	: num_particles(num_particles), particle_radius(radius), cell_dim(cell_dim), relaxation_cell_dim(relaxation_cell_dim), viewport_w(viewport_w), viewport_h(viewport_h), 
	dt(dt), gravity(gravity), obstacle_x(viewport_w), obstacle_y(viewport_h), obstacle_vx(0.0f), obstacle_vy(0.0f), obstacle_r(obstacle_radius), iterations(iterations), 
	flip_ratio(flip_ratio), stiffness_coefficient(stiffness), density_correction(density_correction), over_relaxation(over_relaxation)
{
	cell_rows = static_cast<int>(viewport_h / cell_dim);
	cell_cols = static_cast<int>(viewport_w / cell_dim);

	relaxation_cell_rows = static_cast<int>(viewport_h / relaxation_cell_dim);
	relaxation_cell_cols = static_cast<int>(viewport_w / relaxation_cell_dim);

	relaxation_cell_particle_ids = std::vector<std::vector<std::vector<int>>>(relaxation_cell_rows, std::vector<std::vector<int>>(relaxation_cell_cols));
	cell_types = std::vector<std::vector<CellType>>(cell_rows, std::vector<CellType>(cell_cols, CellType::AIR));
	cell_velocities = std::vector<std::vector<Eigen::Vector2f>>(cell_rows, std::vector<Eigen::Vector2f>(cell_cols, Eigen::Vector2f::Zero()));
	prev_cell_velocities = std::vector<std::vector<Eigen::Vector2f>>(cell_rows, std::vector<Eigen::Vector2f>(cell_cols, Eigen::Vector2f::Zero()));
	cell_colors = std::vector<Eigen::Vector3f>(cell_rows * cell_cols, Eigen::Vector3f::Zero());
	cell_densities = std::vector<std::vector<float>>(cell_rows, std::vector<float>(cell_cols, 0.0f));
	cell_centers_rendering.reserve(cell_rows * cell_cols);

	particle_pos = std::vector<Eigen::Vector2f>(num_particles, Eigen::Vector2f::Zero());
	particle_vel = std::vector<Eigen::Vector2f>(num_particles, Eigen::Vector2f::Zero());
	particle_colors = std::vector<Eigen::Vector3f>(num_particles, Eigen::Vector3f::Ones());

	for (int i = 1; i < cell_rows; ++i)
	{
		cell_types[i][0] = CellType::SOLID;
		cell_types[i][cell_cols - 1] = CellType::SOLID;
	}

	for (int j = 0; j < cell_cols; ++j)
	{
		cell_types[0][j] = CellType::SOLID;
		cell_types[cell_rows - 1][j] = CellType::SOLID;
	}

	// You can modify the way particle positions are initialized,
	// but the layout of the particles should be the same since it is used for rest density calculation.
	float x = cell_dim * 10;
	float y = cell_dim * 10;
	float x_max = 0.5f * viewport_w;

	float dx = 2 * radius;
	float dy = sqrt(3.0) / 2.0 * dx;

	bool stagger = false;

	for (int i = 0; i < num_particles; i++) 
	{
		particle_pos[i] = Eigen::Vector2f(x, y);
		x += dx;
		if (x > x_max)
		{
			y += dy;
			x = stagger ? cell_dim * 10 + radius : cell_dim * 10;
			stagger = !stagger;
		}
	}
}

void Fluid::update(int render_option, bool lmbDown)
{

	// physics
	integrate();
	particleRelaxation();
	handleCollisions(lmbDown);
	transferVelocities(true);
	updateDensity();
	solveIncompressibility();
	transferVelocities(false);

	
	// graphics
	switch (render_option)
	{
	case 0: // cells only
		updateCellColors();
		updateCellColorBuffers();
		renderCells();
		break;
	case 1: // both
		updateCellColors();
		updateCellColorBuffers();
		renderCells();
	case 2: // particles only
		updateParticleColors();
		updateParticleBuffers();
		renderParticles();
		break;
	default:
		return;
	}
	
	renderObstacle();
}

void Fluid::setObstacle(float mouse_x, float mouse_y, float mouse_vx, float mouse_vy)
{
	obstacle_x = mouse_x;
	obstacle_y = viewport_h - mouse_y;
	obstacle_vx = mouse_vx;
	obstacle_vy = -mouse_vy;
}

void Fluid::updateParticleColors()
{
	for (int i = 0; i < num_particles; ++i)
	{
		float vel = particle_vel[i].norm();
		float mult = std::min(vel, 50.0f) / 50.0f;
		particle_colors[i] = Eigen::Vector3f(mult, mult, 1.0f);
	}
}

void Fluid::updateCellColors()
{
	float max_density_estimate = cell_dim / particle_radius;
	for (int i = 0; i < cell_rows; ++i) for (int j = 0; j < cell_cols; ++j)
	{
		int ind = i * cell_cols + j;
		if (cell_types[i][j] == CellType::SOLID)
			cell_colors[ind] = Eigen::Vector3f(0.5f, 0.5f, 0.5f);
		else if (cell_types[i][j] == CellType::FLUID)
		{
			float mult = std::min(cell_densities[i][j], max_density_estimate) / max_density_estimate;
			
			cell_colors[ind] = Eigen::Vector3f(mult, mult, 1.0f);
		}
		else
			cell_colors[ind] = Eigen::Vector3f::Zero();
	}
}

void Fluid::setupRendering(GLuint pointShaders, GLuint obstacleShaders)
{
	// particles
	point_shaders = pointShaders;
	obstacle_shaders = obstacleShaders;

	glGenVertexArrays(1, &particles_vao);
	glBindVertexArray(particles_vao);

	glGenBuffers(1, &particles_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, particles_vbo);
	glBufferData(GL_ARRAY_BUFFER, num_particles * sizeof(Eigen::Vector2f),
		particle_pos.data(), GL_DYNAMIC_DRAW);

	GLint particleAttrPositionLoc = glGetAttribLocation(point_shaders, "attrPosition");
	if (particleAttrPositionLoc != -1) {
		glVertexAttribPointer(particleAttrPositionLoc, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(particleAttrPositionLoc);
	}

	glGenBuffers(1, &particle_colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, particle_colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, num_particles * sizeof(Eigen::Vector3f),
		particle_colors.data(), GL_DYNAMIC_DRAW);

	GLint particleAttrColorLoc = glGetAttribLocation(point_shaders, "attrColor");
	if (particleAttrColorLoc != -1) {
		glVertexAttribPointer(particleAttrColorLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(particleAttrColorLoc);
	}

	glBindVertexArray(0);

	// cells
	int num_cells = cell_rows * cell_cols;
	glGenVertexArrays(1, &cells_vao);
	glBindVertexArray(cells_vao);

	glGenBuffers(1, &cells_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, cells_vbo);

	for (int i = 0; i < cell_rows; ++i)
		for (int j = 0; j < cell_cols; ++j)
			cell_centers_rendering[i * cell_cols + j] = Eigen::Vector2f(j + 0.5, i + 0.5) * cell_dim;

	glBufferData(GL_ARRAY_BUFFER, num_cells * sizeof(Eigen::Vector2f),
		cell_centers_rendering.data(), GL_DYNAMIC_DRAW);

	GLint cellAttrPositionLoc = glGetAttribLocation(point_shaders, "attrPosition");

	glVertexAttribPointer(cellAttrPositionLoc, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(cellAttrPositionLoc);

	glGenBuffers(1, &cell_colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, cell_colors_vbo);

	glBufferData(GL_ARRAY_BUFFER, num_cells * sizeof(Eigen::Vector3f),
		cell_colors.data(), GL_DYNAMIC_DRAW);

	GLint cellAttrColorLoc = glGetAttribLocation(point_shaders, "attrColor");

	glVertexAttribPointer(cellAttrColorLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(cellAttrColorLoc);

	glBindVertexArray(0);

	// obstacle dummy vao
	glGenVertexArrays(1, &obstacle_vao);
}

void Fluid::updateParticleBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, particles_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, num_particles * sizeof(Eigen::Vector2f), particle_pos.data());

	glBindBuffer(GL_ARRAY_BUFFER, particle_colors_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, num_particles * sizeof(Eigen::Vector3f), particle_colors.data());
}

void Fluid::renderParticles()
{
	glUseProgram(point_shaders);

	glUniform2f(glGetUniformLocation(point_shaders, "domainSize"), viewport_w, viewport_h);
	glUniform1f(glGetUniformLocation(point_shaders, "pointSize"), 2.0f * particle_radius);
	glUniform1f(glGetUniformLocation(point_shaders, "drawDisk"), 1.0f);

	glBindVertexArray(particles_vao);
	glDrawArrays(GL_POINTS, 0, num_particles);

	glBindVertexArray(0);
}

void Fluid::updateCellColorBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, cell_colors_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, cell_rows * cell_cols * sizeof(Eigen::Vector3f), cell_colors.data());

}
void Fluid::renderCells()
{
	glUseProgram(point_shaders);

	glUniform2f(glGetUniformLocation(point_shaders, "domainSize"), viewport_w, viewport_h);
	glUniform1f(glGetUniformLocation(point_shaders, "pointSize"), cell_dim);
	glUniform1f(glGetUniformLocation(point_shaders, "drawDisk"), 0.0f);

	glBindVertexArray(cells_vao);
	glDrawArrays(GL_POINTS, 0, cell_rows * cell_cols);

	glBindVertexArray(0);
}

void Fluid::renderObstacle()
{
	glUseProgram(obstacle_shaders);

	glBindVertexArray(obstacle_vao);

	glUniform2f(glGetUniformLocation(obstacle_shaders, "domainSize"), viewport_w, viewport_h);
	glUniform2f(glGetUniformLocation(obstacle_shaders, "attrPosition"), obstacle_x, obstacle_y);
	glUniform1f(glGetUniformLocation(obstacle_shaders, "pointSize"), 2 * obstacle_r);
	
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}



