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

#include "QtUtils.h"

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
    return QString::fromUtf16((const ushort *)str.c_str());
#else
    return QString::fromStdWString(str);
#endif
}

std::string Narrow_WChar(const wchar_t *wstr) {
	std::ostringstream stm;
	const std::ctype<char>& ctfacet = std::use_facet< std::ctype<char> >(stm.getloc());

	const size_t len = wcslen(wstr);
	for (size_t i = 0; i < len; ++i)
		stm << ctfacet.narrow(wstr[i], 0);
	return stm.str();
}

std::wstring Widen_Char(const char *str) {
	std::wostringstream wstm;
	const std::ctype<wchar_t>& ctfacet = std::use_facet<std::ctype<wchar_t> >(wstm.getloc());
	const size_t len = strlen(str);
	for (size_t i = 0; i < len; ++i)
		wstm << ctfacet.widen(str[i]);
	return wstm.str();
}


QUuid GUID_To_QUuid(const GUID& guid) {
	return QUuid(guid.Data1, guid.Data2, guid.Data3, guid.Data4[0], guid.Data4[1], guid.Data4[2],
		guid.Data4[3], guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
}

GUID QUuid_To_GUID(const QUuid& uuid) {
	return { uuid.data1, uuid.data2, uuid.data3,
		uuid.data4[0], uuid.data4[1], uuid.data4[2], uuid.data4[3], uuid.data4[4], uuid.data4[5],
		uuid.data4[6], uuid.data4[7]
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
