# SmartCGMS - continuous glucose monitoring and controlling framework
# https://diabetes.zcu.cz/
#
# Copyright (c) since 2018 University of West Bohemia.
#
# Contact:
# diabetes@mail.kiv.zcu.cz
# Medical Informatics, Department of Computer Science and Engineering
# Faculty of Applied Sciences, University of West Bohemia
# Univerzitni 8, 301 00 Pilsen
# Czech Republic
# 
# 
# Purpose of this software:
# This software is intended to demonstrate work of the diabetes.zcu.cz research
# group to other scientists, to complement our published papers. It is strictly
# prohibited to use this software for diagnosis or treatment of any medical condition,
# without obtaining all required approvals from respective regulatory bodies.
#
# Especially, a diabetic patient is warned that unauthorized use of this software
# may result into severe injure, including death.
#
#
# Licensing terms:
# Unless required by applicable law or agreed to in writing, software
# distributed under these license terms is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#
# a) Without a specific agreement, you are not authorized to make or keep any copies of this file.
# b) For any use, especially commercial use, you must contact us and obtain specific terms and conditions 
#    for the use of the software.
# c) When publishing any derivative work or results obtained using this software, you agree to cite the following paper:
#    Tomas Koutny and Martin Ubl, "SmartCGMS as a Testbed for a Blood-Glucose Level Prediction and/or 
#    Control Challenge with (an FDA-Accepted) Diabetic Patient Simulation", Procedia Computer Science,  
#    Volume 177, pp. 354-362, 2020

CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

SET(PROJ "scgms-common")

PROJECT(${PROJ})

INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/cmake/Functions.cmake")

DISCOVER_DEPENDENCIES()

MESSAGE(STATUS "Configuring common SDK static library: ${PROJ}")

FILE(GLOB iface scgms/iface/*.cpp scgms/iface/*.c scgms/iface/*.hpp scgms/iface/*.h)
FILE(GLOB lang scgms/lang/*.cpp scgms/lang/*.c scgms/lang/*.hpp scgms/lang/*.h)
FILE(GLOB rtl scgms/rtl/*.cpp scgms/rtl/*.c scgms/rtl/*.hpp scgms/rtl/*.h)
FILE(GLOB_RECURSE utils scgms/utils/*.cpp scgms/utils/*.c scgms/utils/*.hpp scgms/utils/*.h)

SOURCE_GROUP(iface FILES ${iface})
SOURCE_GROUP(lang FILES ${lang})
SOURCE_GROUP(rtl FILES ${rtl})
SOURCE_GROUP(utils FILES ${utils})

IF(Qt_DISABLE)
	LIST(FILTER rtl EXCLUDE REGEX ".*qdb_connector.*")
	LIST(FILTER utils EXCLUDE REGEX ".*QtUtils.*")
ELSE()
	DISCOVER_QT_LIBRARY(Core Sql)
ENDIF()

ADD_LIBRARY(${PROJ} STATIC ${iface} ${lang} ${rtl} ${utils})
APPLY_SCGMS_LIBRARY_BUILD_SETTINGS(${PROJ})
CONFIGURE_BASE_LIB_OUTPUT(${PROJ})

IF(NOT Qt_DISABLE)
	TARGET_LINK_LIBRARIES(${PROJ} Qt::Core Qt::Sql)
ENDIF()
