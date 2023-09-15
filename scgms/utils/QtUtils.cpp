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

#include "QtUtils.h"

#ifndef DDO_NOT_USE_QT

#include <sstream>

/*! Convert a QString to an std::wstring */
std::wstring QStringToStdWString(const QString &str) {
#ifdef _MSC_VER
    return std::wstring((const wchar_t *)str.utf16());
#else
    return str.toStdWString();
#endif
}

/*! Convert an std::wstring to a QString */
QString StdWStringToQString(const std::wstring &str) {

#ifdef _MSC_VER
    return QString::fromUtf16((const char16_t *)str.c_str());
#else
    return QString::fromStdWString(str);
#endif
}


QUuid GUID_To_QUuid(const GUID& guid) {
	return QUuid(guid.Data1, guid.Data2, guid.Data3, guid.Data4[0], guid.Data4[1], guid.Data4[2],
		guid.Data4[3], guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
}

GUID QUuid_To_GUID(const QUuid& uuid) {
	return { uuid.data1, uuid.data2, uuid.data3,
		{ uuid.data4[0], uuid.data4[1], uuid.data4[2], uuid.data4[3], uuid.data4[4], uuid.data4[5],
		uuid.data4[6], uuid.data4[7] }
	};
}


#ifndef NOGUI

#include <QtWidgets/QHeaderView>

int HideDbColByName(const QSqlTableModel &model, QTableView &view, const char* dbcolname) {
	int idx = model.fieldIndex(dbcolname);
	if (idx >= 0) view.hideColumn(idx);
	return idx;
};

int SetupDbColUI(QSqlTableModel &model, QTableView &view, const char* dbcolname, const char* uicolname, const int width, const int moveto){
	int idx = model.fieldIndex(dbcolname);
	if (idx >= 0) {
		model.setHeaderData(idx, Qt::Horizontal, QWidget::tr(uicolname));
		view.setColumnWidth(idx, width);

		QHeaderView *hdr = view.horizontalHeader();
		hdr->moveSection(hdr->visualIndex(idx), moveto);
	}
	return idx;
};

#endif

#endif
