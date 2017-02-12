#ifndef NODETREEWIDGET_H
#define NODETREEWIDGET_H

// Foundations
#include "CSceneManager.h"
#include "AView.h"
#include "CMaterialManager.h"
#include "CShaderManager.h"
#include "CMeshManager.h"

class CEditShaderTextItem;
class CParamShaderItem;
class CEditShaderTreeItem;
class CVectorWidgetItem;
class CComboBoxItem;


// Qt
#include <QTreeWidget>
#include <QHeaderView>
#include <QHash>
#include <QComboBox>


class CNodeTreeWidget : public QTreeWidget, 
        public AEntity::IEntityListener,
        public CMaterialManager::IMaterialManagerListener
{
    Q_OBJECT

    friend class CComboBoxItem;
    friend class CParamShaderItem;
    friend class CEditShaderTreeItem;
    friend class CAnimationTreeItem;
    friend class CVectorWidgetItem;

public:

    //! Constructeur
    CNodeTreeWidget(
            CSceneManager* pSceneManager,
            AView* pView,
            CEditShaderTextItem* pEditItem,
            QWidget* parent = 0);

    //! Destructeur
    virtual ~CNodeTreeWidget();

    //! Selectionne un item
    void selectEntity(AEntity* pEntity);

    //! Supprime l'ensemble des donnees
    void clearData();

    //! Met e jour les donnees en fonction du gestionnaire de scene
    void updateData();

signals:

    void editShader(const QString& name);

    void playAnimation(const QString& name, bool bPlay);

protected:

    //! Override - Evenement menu contextuel
    virtual void contextMenuEvent(QContextMenuEvent* pEvent);

    //! Override - Implementation de CEntity::IEntityListener, destruction de l'objet
    virtual void onDelete(AEntity* pEntity);

    //! Override - Implementation de CEntity::IEntityListener, mise e jour de l'objet
    virtual void onUpdate(AEntity* pEntity);

    //! Override - Implementation de CMaterialManager::IMaterialManagerListener
    virtual void onUpdate(CMaterial* pMaterial);

    //! Override - Implementation de CMaterialManager::IMaterialManagerListener
    virtual void onDelete(CMaterial* pMaterial);

private:

    //! Ajout d'un item dans l'arbre
    void addEntity(QTreeWidgetItem* pTreeWidgetItem, AEntity* pEntity);

    //! Cree l'arbre des proprietes du gestionnaire de scene
    void createTreeSceneManager();

    //! Cree l'arbre des proprietes du gestionnaire de rendu
    void createTreeRenderer();

    //! Cree l'arbre des proprietes des noeuds de maniere recursive
    void recursiveCreateTreeNodes(CSceneNode* pNode, QTreeWidgetItem* pParent = 0);

    //! Cree l'arbre des proprietes des materiaux
    void createTreeMaterials(const QList<CMaterial*>& materials);

    //! Cree l'arbre des proprietes des meshs
    void createTreeMeshs(const QList<CMesh*>& meshs);

    //! Cree l'arbre des proprietes des lumières
    void createTreeLights(const QList<CLight*>& lights);

    //! Cree l'arbre des proprietes des caméras
    void createTreeCameras(const QList<CCamera*>& cameras);

    //! Cree l'arbre des proprietes des animations
    void createTreeAnimations(const QList<CAnimation*>& animations);

    //! Cree l'arbre des proprietes des shaders
    void createTreeShaders(const QList<CShader*>& shaders);

    //! Pointeur sur le gestionnaire de scene
    CSceneManager* m_pSceneManager;

    //! Pointeur sur le gestionnaire de rendu
    AView* m_pView;

    //! Conteneur de correspondance entre les items dans l'arbre et les items 3D
    QHash<QTreeWidgetItem*, AEntity*> m_Entities;

    //! Type propriete
    typedef QPair<AEntity*, QString> TProperty;

    //! Conteneur de correspondance entre les items dans l'arbre et les proprietes des items 3D
    QHash<QTreeWidgetItem*, TProperty> m_PropertiesItems;

    //! Ajoute une propriete de type string
    void addProperty(AEntity* pEntity, QTreeWidgetItem* pParent, const QString& name, const QString& property, bool bEditable = true);

    //! Ajoute une propriete de type bool
    void addProperty(AEntity* pEntity, QTreeWidgetItem* pParent, const QString& name, bool bProperty, bool bEditable = true);

    //! Ajoute une propriete de type enum
    void addProperty(AEntity* pEntity, QTreeWidgetItem* pParent, const QString& name, const QList<QString>& properties, const QString& property, bool bEditable = true);

    //! Ajoute une propriete de type vector 3D
    void addProperty(AEntity* pEntity, QTreeWidgetItem* pParent, const QString& name, const QVector3D& property, bool bEditable = true);

    //! Ajoute une propriete de type vector 4D
    void addProperty(AEntity* pEntity, QTreeWidgetItem* pParent, const QString& name, const QVector4D& property, bool bEditable = true);

    //! Ajoute une propriete de type quaternion
    void addProperty(AEntity* pEntity, QTreeWidgetItem* pParent, const QString& name, const QQuaternion& property, bool bEditable = true);

    //! Ajoute une propriete de type fichier
    void addFileProperty(AEntity* pEntity, QTreeWidgetItem* pParent, const QString& name, const QString& property, bool bEditable = true);

    void updateProperty(AEntity* pEntity, const QString& name, const QString& property);

    void updateProperty(AEntity* pEntity, const QString& name, bool bProperty);

    void updateProperty(AEntity* pEntity, const QString& name, const QVector3D& property);

    void updateProperty(AEntity* pEntity, const QString& name, const QVector4D& property);

    //! Recuperer une propriete d'un VectorWidget
    QVector2D getVector2DValue(QTreeWidgetItem* pItem);

    //! Recuperer une propriete d'un VectorWidget
    QVector3D getVector3DValue(QTreeWidgetItem* pItem);

    //! Recuperer une propriete d'un VectorWidget
    QVector4D getVector4DValue(QTreeWidgetItem* pItem);

    //! Recuperer une propriete d'une ComboBox
    QString getComboBoxCurrentEntry(QTreeWidgetItem* pItem);

    //! Ajouter une propriete d'une ComboBox
    void addComboBoxEntry(QTreeWidgetItem* pItem, const QString& entry);

    //! Supprimer une propriete d'une ComboBox
    void removeComboBoxEntry(QTreeWidgetItem* pItem, const QString& entry);

    //! Recuperer une propriete d'une FileSelector
    QString getFileSelectorFilePathName(QTreeWidgetItem* pItem);

    //! Retourne un item representant une propriete en fonction d'un CEntity et du nom de la propriete
    QTreeWidgetItem* getPropertyTreeItem(AEntity* pEntity, const QString& name);

    //! Retourne l'ensemble des items representant une propriete en fonction d'un CEntity
    QList<QTreeWidgetItem*> getPropertiesTreeItems(AEntity* pEntity);

    //! Met e jour un item
    void updateItem(QTreeWidgetItem* pItem);

    //! Action de suppression
    QAction* m_pDeleteAction;

    //! Action de sauvegarde
    QAction* m_pSaveAction;

    //! Action de definition de la camera courante
    QAction* m_pSetCurrentAction;

    //! Entite courante
    AEntity* m_pCurrentEntity;

    //! Pointeur sur l'item qui permet d'editer les shaders
    CEditShaderTextItem* m_pEditItem;

private slots:

    //! Callback de changement sur un item
    void onItemChanged(QTreeWidgetItem*, int);

    //! Callback de mise de jour
    void onUpdate();

    //! Callback de suppression d'un item
    void onDeleteClicked();

    //! Callback de sauvegarde d'un mesh
    void onSaveClicked();

    //! Callback de definition d'une camera courante
    void onSetCurrentClicked();

    //! Callback de mise e jour d'un item
    void onUdpateShader(QTreeWidgetItem* pSender);

    //! Callback de sauvegarde d'un item
    void onSaveShader(QTreeWidgetItem* pSender);
};

#endif // NODETREEWIDGET_H
