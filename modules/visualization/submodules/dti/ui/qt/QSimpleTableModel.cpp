#include "QSimpleTableModel.h"

QSimpleTableModel::QSimpleTableModel(std::vector<std::string> &vertical_header_names, std::vector<std::string> &horizontal_header_names)
{
    for(std::string &name: horizontal_header_names)
        m_horizontalHeaders.append(name.c_str());

    for(std::string &name: vertical_header_names)
        m_verticalHeaders.append(name.c_str());
}

void QSimpleTableModel::updateData(const QList<QList<float> > &table_data)
{
    m_tableData = table_data;
}

int QSimpleTableModel::rowCount(const QModelIndex &/*parent*/) const
{
    return m_verticalHeaders.length();
}

int QSimpleTableModel::columnCount(const QModelIndex &/*parent*/) const
{
    return m_horizontalHeaders.length();
}

QVariant QSimpleTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    if(index.row() < m_tableData.size())
    {
        const QList<float> &row_data = m_tableData[index.row()];
        if(index.column() < row_data.size())
            return row_data[index.column()];
    }

    return QVariant();
}

QVariant QSimpleTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        if(section >= 0 && section < m_horizontalHeaders.length())
            return m_horizontalHeaders[section];
    }

    if(role == Qt::DisplayRole && orientation == Qt::Vertical)
    {
        if(section >= 0 && section < m_verticalHeaders.length())
            return m_verticalHeaders[section];
    }

    return QVariant();
}
