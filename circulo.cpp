#include <osg/Geode>
#include <osg/Geometry>
#include  <osg/Texture2D>
#include <osg/PositionAttitudeTransform>
#include <osgGA/TrackballManipulator>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osgGA/GUIEventHandler>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgUtil/PolytopeIntersector>
#include <osg/Camera>
#include <osg/NodeCallback>
#include <osg/Group>
#include <osg/MatrixTransform>
#include <iostream>
#include <osg/Notify>

using namespace osg;
using namespace std;
using namespace osgDB;


class estadoDelTeclado
{
  public:
   estadoDelTeclado::estadoDelTeclado() :solicitudMovimiento(false) {}
   bool solicitudMovimiento;
};

class miEventosTeclado : public osgGA::GUIEventHandler
{
    public:

        miEventosTeclado(estadoDelTeclado* tids)
        {
          estado = tids;
        }

       virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter&);
       virtual void accept(osgGA::GUIEventHandlerVisitor& v)   { v.visit(*this); };

    protected:

       estadoDelTeclado* estado;
};

bool miEventosTeclado::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa)
{ 
  switch(ea.getEventType())
  {
    case(osgGA::GUIEventAdapter::KEYDOWN):
    {
      switch(ea.getKey())
      {
        case 'w':
          cout << " w key pressed" <<endl;
          estado->solicitudMovimiento = true;
          return false;
        break;

        default:
          return false;
      } 
    }
    default:
      return false;
  }
}

class actualizaPosicion : public osg::NodeCallback 
{
    public:

       actualizaPosicion::actualizaPosicion(estadoDelTeclado* estadoID) 
       {
          estado = estadoID;
       }

     virtual void operator()(Node* node, NodeVisitor* nv)
      {
        int x=0;
		
          PositionAttitudeTransform* pat = dynamic_cast<PositionAttitudeTransform*> (node);
          if(pat)
          {
             if (estado->solicitudMovimiento)
             {  
				       x++;
				       pat->setPosition(Vec3(x, 0, 0));
             }
          }
          traverse(node, nv); 
       }
    protected:
       estadoDelTeclado* estado; 
};

Node *circulo()
{

  Geode *sphere = new Geode();
  sphere->addDrawable(new ShapeDrawable(new Sphere(Vec3(-10, 5, 2), 2.5f)));

  Material *material = new Material();
  material->setDiffuse(Material::FRONT,  Vec4(0, 0, 0, 0));
  material->setSpecular(Material::FRONT, Vec4(0.0, 0.0, 0.0, 1.0));
  material->setAmbient(Material::FRONT,  Vec4(0.1, 0.1, 0.1, 1.0));
  material->setEmission(Material::FRONT, Vec4(0.0, 0.0, 0.0, 1.0));
  material->setShininess(Material::FRONT, 25.0);
  
  Material *rojo = new Material();
  rojo->setDiffuse(Material::FRONT,  Vec4(1, 0, 0, 0));

  sphere->getOrCreateStateSet()->setAttribute(rojo);

  return sphere;
}


int main()
{

  Node *circuloNode = circulo();
	
  estadoDelTeclado* iDestado = new estadoDelTeclado;
       
  circuloNode->setUpdateCallback(new actualizaPosicion(iDestado));

  miEventosTeclado* eventosCirculo = new miEventosTeclado(iDestado);

  osgViewer::Viewer viewer;
  viewer.addEventHandler(eventosCirculo); 
  viewer.setSceneData(circuloNode); 
  return viewer.run();

   
}
