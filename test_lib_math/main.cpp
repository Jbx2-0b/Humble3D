#include <QMatrix4x4>
#include <QTransform>
#include <QImage>
#include <QColor>
#include <QDebug>

#include "Math.h"
#include "CBox3D.h"
#include "CSphere.h"
#include "CSceneManager.h"
#include "CRay.h"
#include "CSceneNode.h"
#include "CNoiseGenerator.h"
#include "CPlane.h"
#include "CCamera.h"
#include "CLight.h"
#include "CFrustum.h"
#include "CArray2D.h"


static QColor c1(255.0, 0.0, 0.0); // Du rouge
static QColor c2(0.0, 255.0, 0.0); // Du vert
static QColor c3(0.0, 0.0, 255.0); // Du bleu

//-----------------------------------------------------------------------------------------
// TESTS UNITAIRES DES OUTILS MATHEMATIQUES
//-----------------------------------------------------------------------------------------
int main(int, char **)
{
    QVector3D cP = QVector3D::crossProduct(QVector3D(2, 3, 4), QVector3D(5, 6, 7));
    qDebug() << cP;

    QQuaternion q = QQuaternion::fromAxisAndAngle(0.f, 0.f, 1.f, 2.f);
    qDebug() << q;

    CCamera cam(0, "TestCam");
    qDebug() << cam.toString();
    cam.setOrtho(0, -800, +600, 0, -1, 1);
    qDebug() << cam.toString();
    cam.setEyePosition(QVector3D(0, 0, -1));
    qDebug() << cam.toString();

    cam.rotateEye(QQuaternion::fromAxisAndAngle(0.f, 0.f, 1.f, 3.f));
    qDebug() << cam.toString();

    cam.rotateEye(QQuaternion::fromAxisAndAngle(0.f, 0.f, 1.f, 3.f));
    qDebug() << cam.toString();

    cam.rotateEye(QQuaternion::fromAxisAndAngle(0.f, 0.f, 1.f, 3.f));
    qDebug() << cam.toString();

    //-----------------------------------------------------------------------------------------
    // Test Random
    //-----------------------------------------------------------------------------------------

    //	int rSize = 300;
    //	QImage randImg(rSize, rSize, QImage::Format_RGB32);

    //	QImage angleImg(rSize, rSize, QImage::Format_RGB32);
    //	angleImg.fill(Qt::white);

    //	for (int i = 0; i < rSize - 1; ++i)
    //	{
    //		for (int j = 0; j < rSize - 1; ++j)
    //		{
    //			real r = Math::rand(QVector2D(i, j));
    //			//qDebug() << r;
    //			randImg.setPixel(i, j, QColor(r * 255., r * 255., r * 255.).rgb());

    //			real cosR = cos(r * 2. * Math::Pi - Math::Pi);
    //			real sinR = sin(r * 2. * Math::Pi - Math::Pi);

    //			//qDebug() << "Angle: " << r * 2. * Math::Pi - Math::Pi; //<< cosR << sinR;

    //			real halfSize = (rSize - 1) / 2;

    //			angleImg.setPixel(halfSize * cosR + halfSize,  halfSize * sinR + halfSize, QColor(Qt::black).rgb());
    //		}
    //	}

    //	randImg.save("C://testN.png");
    //	angleImg.save("C://angle.png");

    //-----------------------------------------------------------------------------------------
    // Test QTransform
    //-----------------------------------------------------------------------------------------

    QTransform T, R, S;
    T.translate(0.8, 0.6);
    R.rotateRadians(1.4);
    S.scale(4.0, 4.0);

    QTransform result = T * R * S;

    //qDebug() << R;
    qDebug() << result;

    //-----------------------------------------------------------------------------------------
    // Test translation d'une boite englobante
    //-----------------------------------------------------------------------------------------
    CBox3D bbox(QVector3D(-1.0, -1.0, -1.0), QVector3D(1.0, 1.0, 1.0));

    QMatrix4x4 translation;
    translation.translate(20., 0., 0.);

    CBox3D transfBBox = bbox.transformed(translation);

    Q_ASSERT(transfBBox.getMinimum().x() == 19. && transfBBox.getMaximum().x() == 21.);

    //-----------------------------------------------------------------------------------------
    // Test rotation d'une boite englobante
    //-----------------------------------------------------------------------------------------
    QMatrix4x4 rotation;
    rotation.rotate(45, 1., 0.);

    CBox3D transfBBox2 = bbox.transformed(rotation);

    real dEpsilon = 0.00001;

    Q_ASSERT(Math::isEqual(transfBBox2.getMinimum().x(), -1., dEpsilon) && Math::isEqual(transfBBox2.getMaximum().x(), 1., dEpsilon));

    //-----------------------------------------------------------------------------------------
    // Test translation union de deux boites englobantes
    //-----------------------------------------------------------------------------------------
    CBox3D bbox2(QVector3D(1.0, 1.0, 1.0), QVector3D(2.0, 2.0, 2.0));

    CBox3D unitedBBox = bbox.united(bbox2);

    Q_ASSERT(unitedBBox.getMinimum().x() == -1. && unitedBBox.getMaximum().x() == 2.);

    //-----------------------------------------------------------------------------------------
    // Test translation union avec boîte englobante nulle
    //-----------------------------------------------------------------------------------------
    CBox3D bbox3;

    unitedBBox = unitedBBox.united(bbox3);

    Q_ASSERT(unitedBBox.getMinimum().x() == -1. && unitedBBox.getMaximum().x() == 2.);

    //-----------------------------------------------------------------------------------------
    // Test d'intersection rayon / boîte englobante
    //-----------------------------------------------------------------------------------------
    CRay ray(QVector3D(0., 0., -4.), QVector3D(0., 0., 1.));

    Q_ASSERT(bbox.intersects(ray));
    Q_ASSERT(!transfBBox.intersects(ray));

    CRay ray2(QVector3D(-3., 0., 0.), QVector3D(1., 0., 0.));

    Q_ASSERT(bbox.intersects(ray2));
    Q_ASSERT(transfBBox.intersects(ray2));

    CRay ray3(QVector3D(0., 3., 0.), QVector3D(1., 0., 0.));

    Q_ASSERT(!bbox.intersects(ray3));
    Q_ASSERT(!transfBBox.intersects(ray3));


    //-----------------------------------------------------------------------------------------
    // Test de transformation recursive par noeud (translation)
    //-----------------------------------------------------------------------------------------
    CSceneNode root(new CSceneManager(), "Root");

    CSceneNode* pChild = root.createChild("Child", translation);

    root.dumpNodeTree();

    CBox3D transfByNodeBBox = bbox.transformed(pChild->getGlobalTransformation());

    Q_ASSERT(transfBBox == transfByNodeBBox);

    //-----------------------------------------------------------------------------------------
    // Test de transformation recursive par noeud (rotation)
    //-----------------------------------------------------------------------------------------

    CSceneNode* pChild2 = root.createChild("Child2", rotation);

    CBox3D transfByNodeBBox2 = bbox.transformed(pChild2->getGlobalTransformation());

    Q_ASSERT(transfBBox2 == transfByNodeBBox2);

    //-----------------------------------------------------------------------------------------
    // Test generateur de bruit de perlin
    //-----------------------------------------------------------------------------------------

    //	int iSize = 512;
    //	double dPersistance = 0.5;
    //	QImage img(iSize, iSize, QImage::Format_RGB32);

    //	CNoiseGenerator2D ng(iSize, iSize, iSize / 2, 8);
    //  ng.toImage(dPersistance, QList<QColor>() << Qt::red << Qt::green << Qt::blue, QList<double>() << 0.25 << 0.5 << 0.75).save("C://testo.png");

    //-----------------------------------------------------------------------------------------
    // Test distance point-plan
    //-----------------------------------------------------------------------------------------

    CPlane plane(QVector3D(3., 0., 0.), QVector3D(3., 1., 0.), QVector3D(3., 0., 1.));

    Q_ASSERT(Math::isEqual(plane.getDistance(QVector3D(5.2, 1., 0.)), 2.2, dEpsilon));

    QVector3D pV1 = QVector3D(2.5, 0., 0.);
    QVector3D pV2 = QVector3D(2.5, 0., 1.);
    QVector3D pV3 = QVector3D(0., 2.5, 0.);

    CPlane plane2(pV1, pV2, pV3);

    Q_ASSERT(Math::isEqual(((pV1 + pV3) / 2.).length(), plane2.getDistance(QVector3D(0., 0., 0.)), dEpsilon));

    //-----------------------------------------------------------------------------------------
    // Test intersection frustum-box
    //-----------------------------------------------------------------------------------------

    CCamera camera(0, "MyCam");
    camera.setEyePosition(QVector3D(0., 0., 0.));
    camera.setPerspective(Math::Pi/2., 1., 2., 6.);
    camera.setCenter(QVector3D(1., 0., 0.));

    CFrustum frustum = camera.getFrustum();
    //	Q_ASSERT(frustum.isPointInFrustum(QVector3D(0., 4., 0.)) == eOutside);
    //	Q_ASSERT(frustum.isPointInFrustum(QVector3D(0., 0., 0.)) == eOutside);
    //	Q_ASSERT(frustum.isPointInFrustum(QVector3D(4., 0., 0.)) == eInside);

    CBox3D myBBox0(QVector3D(0., 3., 0.), QVector3D(1., 4., 1.));
    CBox3D myBBox1(QVector3D(5., 5., 0.), QVector3D(6., 6., 1.));
    CBox3D myBBox2(QVector3D(5., 0., 0.), QVector3D(6., 1., 1.));
    CBox3D myBBox3(QVector3D(-1., -9., 0.), QVector3D(9., 9., 1.));
    CBox3D myBBox4(QVector3D(-5., 0., -1.), QVector3D(-2., 1., 2.));
    CBox3D myBBox5(QVector3D(8., -1, -1.), QVector3D(11., 1., 2.));
    CBox3D myBBox6(QVector3D(5., -1., -1.), QVector3D(8., 0., 2.));
    CBox3D myBBox7(QVector3D(0., 0., -1.), QVector3D(1., 1., 1.));
    CBox3D myBBox8(QVector3D(3., -2., 0.), QVector3D(4., -1., 1.));


    QList<CBox3D> bboxList;

    for (int i = 0; i < 1000; ++i)
    {
        bboxList << CBox3D(
                        QVector3D(Math::randDouble(-10, 0), Math::randDouble(-10, 0), Math::randDouble(-10, 0)),
                        QVector3D(Math::randDouble(0, 10), Math::randDouble(0, 10), Math::randDouble(0, 10)));
    }

    QTime timer;

    timer.start();
    for (const CBox3D& box : bboxList)
    {
        frustum.isVisible(box);
    }
    qDebug() << "IsVisible: " << timer.elapsed();

    timer.start();
    for (const CBox3D& box : bboxList)
    {
        frustum.isBoxInFrustum(box);
    }
    qDebug() << "IsBoxInFrustum: " << timer.elapsed();


    Q_ASSERT(frustum.isBoxInFrustum(myBBox0) == eOutside);
    Q_ASSERT(!frustum.isVisible(myBBox0));

    Q_ASSERT(frustum.isBoxInFrustum(myBBox1) == eIntersect);
    Q_ASSERT(frustum.isSphereInFrustum(CSphere(myBBox1)) == eIntersect);
    Q_ASSERT(frustum.isVisible(myBBox1));

    Q_ASSERT(frustum.isBoxInFrustum(myBBox2) == eInside);
    Q_ASSERT(frustum.isSphereInFrustum(CSphere(myBBox2)) == eIntersect);
    Q_ASSERT(frustum.isVisible(myBBox2));

    Q_ASSERT(frustum.isBoxInFrustum(myBBox3) == eIntersect);
    Q_ASSERT(frustum.isSphereInFrustum(CSphere(myBBox3)) == eIntersect);
    Q_ASSERT(frustum.isVisible(myBBox3));

    Q_ASSERT(frustum.isBoxInFrustum(myBBox4) == eOutside);
    Q_ASSERT(!frustum.isVisible(myBBox4));

    Q_ASSERT(frustum.isBoxInFrustum(myBBox5) == eOutside);
    Q_ASSERT(!frustum.isVisible(myBBox5));

    Q_ASSERT(frustum.isBoxInFrustum(myBBox6) == eIntersect);
    Q_ASSERT(frustum.isSphereInFrustum(CSphere(myBBox6)) == eIntersect);
    Q_ASSERT(frustum.isVisible(myBBox6));

    Q_ASSERT(frustum.isBoxInFrustum(myBBox7) == eOutside);
    Q_ASSERT(!frustum.isVisible(myBBox7));

    Q_ASSERT(frustum.isBoxInFrustum(myBBox8) == eInside);
    Q_ASSERT(frustum.isSphereInFrustum(CSphere(myBBox8)) == eInside);
    Q_ASSERT(frustum.isVisible(myBBox8));


    //-----------------------------------------------------------------------------------------
    // Tools
    //-----------------------------------------------------------------------------------------

    QMatrix4x4 infPerspective = Math::infinitePerspective(Math::degToRad(50.0), 4./3., 0.5);

    qDebug() << infPerspective;

    //-----------------------------------------------------------------------------------------
    // Test tableaux
    //-----------------------------------------------------------------------------------------

    CArray2D<int> array(3, 3);

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            array(i, j) = Math::randInt(0, 10);
        }
    }

    array.dump();

    CArray2D<int> scaled = array.scaled(5, 5);

    scaled.dump();

    return true;
}
