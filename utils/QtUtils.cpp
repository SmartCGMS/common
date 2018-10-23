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
 *       monitoring", Proceedings of the 8th International Conference on Current
 *       and Future Trends of Information and Communication Technologies
 *       in Healthcare (ICTH 2018) November 5-8, 2018, Leuven, Belgium
 */

#include "QtUtils.h"

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
