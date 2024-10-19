#pragma once
#include "Transform.h"

namespace Plato {
class WorldObject;
class Renderer;

/** Base class for component-scripts that can be added to world objects.
 */
class Component {
public:
  //! Handle to the corresponding world object
  WorldObject *const worldObject;
  //! Handle to the corresponding transform
  Transform *const transform;

  //! Will return whether or not this component is locally enabled
  bool GetIsEnabled() const;

  //! Will return whether or not this component is globally enabled (respecting
  //! its world objects and its parents disabled-states)
  bool GetIsGloballyEnabled() const;

  //! Will set the enabled-status of this component. Disabled components receive
  //! no hook calls!
  void SetIsEnabled(bool state);

  //! Will locally enable this component. Disabled components receive no hook
  //! calls!
  void Enable();

  //! Will locally disable this component. Disabled components receive no hook
  //! calls!
  void Disable();

protected:
  virtual ~Component(){};

  // ### GAME ENGINE HOOKS ### //

  // These hooks are not defined as abstract ( = 0 ), because they should not
  // have to be implemented. A component that just uses Update(double) should be
  // fine by just implementing Update(double).

  //! Gets called once after the object is all set up
  virtual void Init() {};

  //! Gets called every frame
  virtual void Update(double frametime) {};

  //! Gets called every frame, right after Update() has been called on every
  //! Component
  virtual void LateUpdate(double frametime) {};

  //! Gets called every frame just before rendering Register your Renderables in
  //! renderer
  virtual void Render(Renderer *renderer) {};

  //! Gets called just before this object gets deleted
  virtual void OnDestroy() {};

protected:
  Component(WorldObject *worldObject);

private:
  bool enabled = true;

  friend class WorldObject;
  // Allow access to call hooks
  friend class WorldObjectManager;
};
} // namespace Plato
