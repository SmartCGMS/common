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
 * Univerzitni 8
 * 301 00, Pilsen
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
 * a) For non-profit, academic research, this software is available under the
 *      GPLv3 license.
 * b) For any other use, especially commercial use, you must contact us and
 *       obtain specific terms and conditions for the use of the software.
 * c) When publishing work with results obtained using this software, you agree to cite the following paper:
 *       Tomas Koutny and Martin Ubl, "Parallel software architecture for the next generation of glucose
 *       monitoring", Procedia Computer Science, Volume 141C, pp. 279-286, 2018
 */

#include "../iface/DeviceIface.h"
#include "SensorLib.h"
#include "FactoryLib.h"
#include "FilterLib.h"
#include "UILib.h"

namespace glucose
{
	namespace imported {
		glucose::TGet_Device_Driver_Descriptors get_device_driver_descriptors = factory::resolve_symbol<glucose::TGet_Device_Driver_Descriptors>("get_device_driver_descriptors");
		glucose::TCreate_Device_Driver create_device_driver = factory::resolve_symbol<glucose::TCreate_Device_Driver>("create_device_driver");
	}

	std::vector<TDevice_Driver_Descriptor> get_device_driver_descriptors()
	{
		std::vector<TDevice_Driver_Descriptor> result;
		TDevice_Driver_Descriptor *desc_begin, *desc_end;

		if (imported::get_device_driver_descriptors(&desc_begin, &desc_end) == S_OK) {
			std::copy(desc_begin, desc_end, std::back_inserter(result));
		}

		return result;
	}

	SDevice_Driver create_device_driver(const GUID &id, glucose::SFilter& output) {
		SDevice_Driver result;
		IDevice_Driver *device_driver;

		if (imported::create_device_driver(&id, output.get(), &device_driver) == S_OK)
			result = refcnt::make_shared_reference_ext<SDevice_Driver, IDevice_Driver>(device_driver, false);

		return result;
	}
}
