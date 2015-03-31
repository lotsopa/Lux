# What is Lux? #

Lux is a personal side project I started working on in the summer of 2014. It started out as a Component Entity System, but grew into a graphics platform agnostic engine. The whole engine is heavily inspired by the Unity 3D game engine.

It uses memory pools in order to maximize cache coherency, a handle based resouce management to ensure no memory leaks are introduced, as well as a clean and easy to extend API. It comes with some built-in component types, however the user can easily add their own, thanks to the latest C++11 features and functional programming principles.

Switching renderers is as easy as changing an enumerator in the application initialization parameters, no strings attached. The sample application shows both the DirectX 11 and OpenGL 4.5 renderer. Lux is still in active development and more exciting features are planned.

### Features ###

* OpenGL 4.5 Renderer
* DirectX 11 Renderer
* Loading assets from archives ( .zip, .rar, .7zip)
* Component Entity Architecture
* Support for any type of custom component
* Behaviour System
* Memory Pools to optimize cache coherency

### Contact ###

* Email: apostol_dadachev@yahoo.com
* Twitter: @ApostolDadachev
* Facebook: Apostol Dadachev