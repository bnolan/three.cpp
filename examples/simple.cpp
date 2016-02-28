#include <assert.h>
#include <stdio.h>
#include <string>
#include <cmath>

#include "common.h"

#include "three/core/geometry.h"
#include "three/cameras/perspective_camera.h"
#include "three/lights/point_light.h"
#include "three/objects/mesh.h"
#include "three/extras/geometries/sphere_geometry.h"
#include "three/extras/geometries/box_geometry.h"
#include "three/extras/geometries/plane_geometry.h"
#include "three/materials/mesh_lambert_material.h"

#include "easywsclient.hpp"
#include "easywsclient.cpp"

#include "pugixml.hpp"
#include "pugixml.cpp"

#include <thread>
#include <map>

#include "examples/extras/stats.h"

using namespace three;
using namespace three_examples;

using namespace std;

using easywsclient::WebSocket;
static WebSocket::pointer websocket = NULL;

Scene::Ptr scene;
std::mutex scene_mutex;

bool QUITTING = false;

class Element{
  public:
    pugi::xml_node *node;
    Object3D::Ptr object;

    Element(pugi::xml_node *n, Object3D::Ptr o){
      node = n;
      object = o;
    }
};

Vector3 parseVector(const std::string str) {
  istringstream s2(str);
  Vector3 v;

  s2 >> v.x;
  s2 >> v.y;
  s2 >> v.z;

  return v;  
}

map<string, shared_ptr<Element>> elementMap;

void handle_message(const std::string & message)
{
  // printf(">>> %s\n", message.c_str());

  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_string(message.c_str());

  if (result)
  {
    std::lock_guard<std::mutex> guard(scene_mutex);

    // Materials
    auto material = MeshLambertMaterial::create(
      Material::Parameters().add( "color", Color( 0xcccccc ) )
    );

    for (auto node = doc.child("packet").first_child(); node; node = node.next_sibling()) {
      string uuid = node.attribute("uuid").value();

      Vector3 position = parseVector(node.attribute("position").value());
      Vector3 scale = parseVector(node.attribute("scale").value());

      if (elementMap.find(uuid) != elementMap.end()) {
        auto element = elementMap[uuid];

        if (strcmp(node.name(), "dead") == 0) {
          cout << "removed\n";


        }

        element->object->position().copy(position);
        element->object->scale().copy(scale);
      } else {
        // Geometries
        auto geometry = BoxGeometry::create( 1, 1, 1 );
        auto object = Mesh::create(geometry, material);
        auto element = std::make_shared<Element>(&node, object);

        object->position().copy(position);
        object->scale().copy(scale);

        elementMap[uuid] = element;

        scene->add(object);
      }
    }

    // string uuid = 
    //   std::cout << "XML [" << message.c_str() << "] parsed without errors, attr value: [" << 
    //     doc.child("packet").first_child().attribute("uuid").value()
    //   << "]\n\n";
  }

}

void connectToServer () {
  //ws->send("hello");
  //ws->send("goodbye");
  //while ((!QUITTING) && (websocket->getReadyState() != WebSocket::CLOSED)) {
    //websocket->poll(200);
    // websocket->dispatch(handle_message);
  //}

  //websocket->close();
  //delete websocket;
}

void simple( GLWindow& window, GLRenderer& renderer ) {

  // Camera
  auto camera = PerspectiveCamera::create(
    50, ( float )renderer.width() / renderer.height(), .1f, 1000.f
  );
  camera->position().z = 10;


  // Scene
  scene = Scene::create();
  scene->add( camera );


  // Lights
  auto pointLight = PointLight::create( 0xFFFFFF );
  pointLight->position() = Vector3( 0.75, 1, 0.5 ).multiplyScalar(50);
  pointLight->lookAt(Vector3(0, 0, 0));
  scene->add( pointLight );


  // Materials
  auto material = MeshLambertMaterial::create(
    Material::Parameters().add( "color", Color( 0xffffff ) )
  );


  // Geometries
  auto geometry = PlaneGeometry::create( 64, 64 );

  auto plane = Mesh::create( geometry, material );
  plane->rotation().x = -M_PI / 2;
  scene->add( plane );


  // Events
  auto mouseX = 0.f, mouseY = 0.f;
  window.addEventListener(SDL_MOUSEMOTION, [&]( const SDL_Event& event ) {
    mouseX = 2.f * ((float)event.motion.x / renderer.width()  - 0.5f);
    mouseY = 2.f * ((float)event.motion.y / renderer.height() - 0.5f);
  });

  // Rendering

  std::thread t1(connectToServer);

  window.animate( [&]( float dt ) -> bool {
    std::lock_guard<std::mutex> guard(scene_mutex);

    camera->position().x += (-10.f * mouseX - camera->position().x ) * 3 * dt;
    camera->position().y = 2.0f;
    camera->lookAt( scene->position() );

    renderer.render( *scene, *camera );

    return true;

  } );

  printf("Quitting...");
  QUITTING = true;
  t1.join();


}

int main( int argc, char* argv[] ) {
  websocket = WebSocket::from_url("ws://scenevr-welcome.herokuapp.com/hello.xml"); // "wss://grid.scenevr.com/scenes/24");

  RendererParameters parameters;
  parameters.antialias = true;

  return RunExample( simple, parameters );

}
