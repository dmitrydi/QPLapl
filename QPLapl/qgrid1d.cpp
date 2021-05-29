#include "qgrid1d.h"

QGrid1D::QGrid1D(const QString& varTitle, LogDirection direction, QWidget *parent): QWidget(parent)
                                                            , direction(direction)
                                                            , varTitle(varTitle)
                                                            , hLayout(new QHBoxLayout(this))
{
    titleLabel = new QLabel();
    hLayout->addWidget(titleLabel);
    titleLabel->setText(varTitle);
    lineEdit = new QLineEdit();
    hLayout->addWidget(lineEdit);
    LinLogBox = new QComboBox();
    hLayout->addWidget(LinLogBox);
    LinLogBox->addItems(gridType);
}

GridSettings QGrid1D::GetGridSettings() const {
    int nsteps = lineEdit->text().toInt();
    Q_ASSERT(nsteps > 0);
    if (LinLogBox->currentText() == "Lin") {
        return {QGridType::Lin, nsteps, std::nullopt};
    } else if (LinLogBox->currentText() == "Log") {
        return {QGridType::Log, nsteps, direction};
    } else {
        throw std::invalid_argument("QGrid1D::GetGridSettings(): unknown grid type\n");
    }
}

std::vector<double> LinLogGrid(double xmin, double xmax, GridSettings settings, double factor) {
    int nseg = settings.nSegments;
    if (settings.gType == QGridType::Lin) {
        return LinSpaced(xmin, xmax, nseg + 1);
    } else {
        double dist, d1;
        dist = std::abs(xmax-xmin);
        if ( factor != 1.) {
            double fact_n = std::pow(factor, nseg);
            d1 = dist*(1. - factor)/(1. - fact_n);
        } else {
            d1 = dist/nseg;
        }
        std::vector<double> ans(nseg+1);
        if (*(settings.direction) == LogDirection::MaxToMin) {
            ans[0] = xmin;
            for (int i = 1; i <= nseg; ++i) {
                ans[i] = ans[i-1] + d1;
                d1 *= factor;
            }
        } else {
            ans[nseg] = xmax;
            for (int i = nseg - 1; i >= 0; --i) {
                ans[i] = ans[i+1] - d1;
                d1 *= factor;
            }
        }
        return ans;
    }
}
