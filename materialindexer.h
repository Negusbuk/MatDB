#ifndef MATERIALINDEXER_H
#define MATERIALINDEXER_H

#include <unordered_set>
#include <unordered_map>
#include <string>

#include <QObject>

#include <material.h>

class MaterialListModel;

class MaterialIndexer : public QObject
{
    Q_OBJECT
public:
    explicit MaterialIndexer(MaterialListModel *listmodel,
                             QObject *parent = 0);

    void filter(const QString& key, std::vector<Material*>& materials);

signals:

public slots:

    void materialCountChanged(int count);
    void materialMetadataChanged(Material*material);

    void dumpIndex();

protected:
    MaterialListModel* ListModel_;
    typedef std::map<std::string,std::unordered_set<Material*> > keyMapType;
    keyMapType keyMap_;
    typedef std::map<Material*,std::unordered_set<std::string> > materialMapType;
    materialMapType materialMap_;

    void processKey(const std::string& key, Material*material);
};

#endif // MATERIALINDEXER_H
