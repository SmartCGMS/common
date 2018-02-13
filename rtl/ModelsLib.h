#pragma once

//This header files only advertises known ids, etc. of different models and solvers to allow further optimization

namespace diffusion_v2_model {
	constexpr size_t param_count = 6;	//p, cg, c, dt, k, h
}

namespace steil_rebrin {
	constexpr size_t param_count = 4;	//g=1 => tau, alpha, beta, gamma
}

