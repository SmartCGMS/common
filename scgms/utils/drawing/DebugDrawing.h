/**
 * SmartCGMS - continuous glucose monitoring and controlling framework
 * https://diabetes.zcu.cz/
 *
 * Copyright (c) since 2018 University of West Bohemia.
 *
 * Contact:
 * diabetes@mail.kiv.zcu.cz
 * Medical Informatics, Department of Computer Science and Engineering
 * Faculty of Applied Sciences, University of West Bohemia
 * Univerzitni 8, 301 00 Pilsen
 * Czech Republic
 * 
 * 
 * Purpose of this software:
 * This software is intended to demonstrate work of the diabetes.zcu.cz research
 * group to other scientists, to complement our published papers. It is strictly
 * prohibited to use this software for diagnosis or treatment of any medical condition,
 * without obtaining all required approvals from respective regulatory bodies.
 *
 * Especially, a diabetic patient is warned that unauthorized use of this software
 * may result into severe injure, including death.
 *
 *
 * Licensing terms:
 * Unless required by applicable law or agreed to in writing, software
 * distributed under these license terms is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *
 * a) This file is available under the Apache License, Version 2.0.
 * b) When publishing any derivative work or results obtained using this software, you agree to cite the following paper:
 *    Tomas Koutny and Martin Ubl, "SmartCGMS as a Testbed for a Blood-Glucose Level Prediction and/or 
 *    Control Challenge with (an FDA-Accepted) Diabetic Patient Simulation", Procedia Computer Science,  
 *    Volume 177, pp. 354-362, 2020
 */

#pragma once

#include "SVGRenderer.h"
#include "Drawing.h"

#include "../math_utils.h"

#include <map>
#include <optional>

/*
 * Namespace holding implementations related to optional debug drawing
 */
namespace debug_drawing {

	// value pair
	struct TValue {
		double time;
		double value;
	};

	/*
	 * Debug drawing class for a single image
	 */
	class CDebug_Drawing {
		private:
			std::vector<TValue> mValues;

			std::optional<double> mCustom_Min_X;
			std::optional<double> mCustom_Max_X;
			std::optional<double> mCustom_Min_Y;
			std::optional<double> mCustom_Max_Y;

		public:
			void Push_Value(double time, double value) {
				if (Is_Any_NaN(time, value)) {
					return;
				}
				mValues.push_back({ time, value });
			}

			void Set_Custom_Min_X(double minX) {
				mCustom_Min_X = minX;
			}
			void Set_Custom_Max_X(double maxX) {
				mCustom_Max_X = maxX;
			}
			void Set_Custom_Min_Y(double minY) {
				mCustom_Min_Y = minY;
			}
			void Set_Custom_Max_Y(double maxY) {
				mCustom_Max_Y = maxY;
			}

			void Render_Group(drawing::Group& grp, size_t width, size_t height, size_t offsetX = 0, size_t offsetY = 0);
			void Render_SVG(const std::string& filename, size_t width, size_t height);
	};

	/*
	 * Coupled drawing class for multiple images in a single SVG
	 */
	class CCoupled_Drawing {
		private:
			std::map<std::string, CDebug_Drawing> mDrawings;

		public:
			CDebug_Drawing& Get(const std::string& id) {
				return mDrawings[id];
			}

			void Render_SVG(const std::string& filename, size_t singleWidth, size_t singleHeight);
	};

}
