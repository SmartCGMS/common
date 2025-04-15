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

#include "DebugDrawing.h"

#undef min
#undef max

#include <fstream>

namespace debug_drawing
{
	void CDebug_Drawing::Render_SVG(const std::string& filename, size_t width, size_t height) {

		drawing::Drawing mDrawing;

		drawing::Group& line_group = mDrawing.Root().Add<drawing::Group>();

		Render_Group(line_group, width, height);

		std::string target;
		CSVG_Renderer renderer(static_cast<double>(width), static_cast<double>(height), target);

		mDrawing.Render(renderer);
		std::ofstream file(filename);
		if (file.is_open()) {
			file << target;
		}
	}

	void CDebug_Drawing::Render_Group(drawing::Group& line_group, size_t width, size_t height, size_t offsetX, size_t offsetY) {
		// draw signals in mValues, scale time and value to fit the drawing
		double min_time = mCustom_Min_X.value_or(mValues[0].time);
		double max_time = mCustom_Max_X.value_or(mValues[0].time);
		double min_value = mCustom_Min_Y.value_or(mValues[0].value);
		double max_value = mCustom_Max_Y.value_or(mValues[0].value);
		for (const auto& value : mValues) {
			if (!mCustom_Min_X.has_value()) {
				min_time = std::min(min_time, value.time);
			}
			if (!mCustom_Max_X.has_value()) {
				max_time = std::max(max_time, value.time);
			}
			if (!mCustom_Min_Y.has_value()) {
				min_value = std::min(min_value, value.value);
			}
			if (!mCustom_Max_Y.has_value()) {
				max_value = std::max(max_value, value.value);
			}
		}

		const double time_range = max_time - min_time;
		const double value_range = max_value - min_value;
		const double scale_x = static_cast<double>(width) / time_range;
		const double scale_y = static_cast<double>(height) / value_range;

		const double offset_x = -min_time * scale_x;
		const double offset_y = -min_value * scale_y;
		const double scale = std::min(scale_x, scale_y);
		const double translate_x = offset_x + (width - time_range * scale) / 2.0;
		const double translate_y = offset_y + (height - value_range * scale) / 2.0;

		// draw the lines
		line_group.Set_Fill_Color(RGBColor(0, 0, 0));

		for (size_t i = 0; i < mValues.size() - 1; ++i) {
			const auto& start = mValues[i];
			const auto& end = mValues[i + 1];
			drawing::Line& line = line_group.Add<drawing::Line>();

			const double trim_start_time = std::max(min_time, std::min(start.time, max_time));
			const double trim_end_time = std::min(max_time, std::max(end.time, min_time));
			const double trim_start_value = std::max(min_value, std::min(start.value, max_value));
			const double trim_end_value = std::min(max_value, std::max(end.value, min_value));

			line.Set_Position(offsetX + (trim_start_time - min_time) * scale_x, offsetY + height - (trim_start_value - min_value) * scale_y);
			line.Set_Target_X(offsetX + (trim_end_time - min_time) * scale_x);
			line.Set_Target_Y(offsetY + height - (trim_end_value - min_value) * scale_y);
			line.Set_Stroke_Width(2.0);
			line.Set_Stroke_Color(RGBColor::From_HTML_Color("#00AA00"));
		}
	}

	void CCoupled_Drawing::Render_SVG(const std::string& filename, size_t singleWidth, size_t singleHeight) {
		drawing::Drawing mDrawing;

		const size_t drawingCount = mDrawings.size();
		if (drawingCount == 0) {
			return;
		}

		// calculate drawing count per line, assuming we want them approximatelly in square grid
		size_t lineCount = static_cast<size_t>(std::sqrt(static_cast<double>(drawingCount)));
		size_t lineWidth = drawingCount / lineCount;
		size_t lineHeight = drawingCount / lineCount;
		if (lineCount * lineWidth < drawingCount) {
			lineHeight++;
		}

		if (lineCount * lineHeight < drawingCount) {
			lineWidth++;
		}

		const size_t totalWidth = lineWidth * singleWidth;
		const size_t totalHeight = lineHeight * singleHeight;

		size_t i = 0;
		for (auto& drawing : mDrawings) {
			drawing::Group& line_group = mDrawing.Root().Add<drawing::Group>();

			// position groups to the grid
			size_t x = i % lineWidth;
			size_t y = i / lineWidth;

			line_group.Add<drawing::Text>(static_cast<double>(x * singleWidth + singleWidth / 2 - 8), static_cast<double>(y * singleHeight + 36), drawing.first).Set_Font_Size(32.0).Set_Fill_Color(RGBColor(0, 0, 0));

			// draw a rectangle around the group
			drawing::Rectangle& rect = line_group.Add<drawing::Rectangle>();
			rect.Set_Position(static_cast<double>(x * singleWidth), static_cast<double>(y * singleHeight));
			rect.Set_Width(static_cast<double>(singleWidth));
			rect.Set_Height(static_cast<double>(singleHeight));
			rect.Set_Stroke_Width(1.0);
			rect.Set_Stroke_Color(RGBColor(0, 0, 0));
			rect.Set_Fill_Opacity(0.0);
			rect.Set_Fill_Color(RGBColor(255, 255, 255));

			drawing.second.Render_Group(line_group, singleWidth, singleHeight, x * singleWidth, y * singleHeight);

			i++;
		}

		std::string target;
		CSVG_Renderer renderer(static_cast<double>(singleWidth * lineWidth), static_cast<double>(singleHeight * lineHeight), target);
		mDrawing.Render(renderer);
		std::ofstream file(filename);
		if (file.is_open()) {
			file << target;
		}
	}
}
