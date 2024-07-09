#ifndef QSIMPLE_TABLE_MODEL_H
#define QSIMPLE_TABLE_MODEL_H

#include <QAbstractTableModel>

class QSimpleTableModel: public QAbstractTableModel
{
    Q_OBJECT

public:
    QSimpleTableModel(std::vector<std::string> &vertical_header_names, std::vector<std::string> &horizontal_header_names);

    void updateData(const QList<QList<float> > &data_table);

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

private:
    QList<QString> m_horizontalHeaders;
    QList<QString> m_verticalHeaders;
    QList<QList<float> > m_tableData;
};

#endif // QSIMPLE_TABLE_MODEL_H
