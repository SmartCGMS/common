/**
 * SmartCGMS - continuous glucose monitoring and controlling framework
 * https://diabetes.zcu.cz/
 *
 * Contact:
 * diabetes@mail.kiv.zcu.cz
 * Medical Informatics, Department of Computer Science and Engineering
 * Faculty of Applied Sciences, University of West Bohemia
 * Technicka 8
 * 314 06, Pilsen
 *
 * Licensing terms:
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *
 * a) For non-profit, academic research, this software is available under the
 *    GPLv3 license. When publishing any related work, user of this software
 *    must:
 *    1) let us know about the publication,
 *    2) acknowledge this software and respective literature - see the
 *       https://diabetes.zcu.cz/about#publications,
 *    3) At least, the user of this software must cite the following paper:
 *       Parallel software architecture for the next generation of glucose
 *       monitoring, Proceedings of the 8th International Conference on Current
 *       and Future Trends of Information and Communication Technologies
 *       in Healthcare (ICTH 2018) November 5-8, 2018, Leuven, Belgium
 * b) For any other use, especially commercial use, you must contact us and
 *    obtain specific terms and conditions for the use of the software.
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
