// Local
#include "LayersSelectionModel.h"
#include "LayersModel.h"

using namespace KIPIPhotoFramesEditor;

LayersSelectionModel::LayersSelectionModel(LayersModel * model, QObject *parent) :
    QItemSelectionModel(model, parent)
{
}
