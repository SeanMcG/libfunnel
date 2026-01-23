# libfunnel Documentation {#mainpage}

libfunnel is a library to make creating PipeWire video streams easy, using zero-copy DMA-BUF frame sharing. "Spout2 / Syphon, but for Linux".

@section Lifetimes

This documentation uses the following terms to describe the lifetime of API objects:

For input arguments (the default if not specified):

* @b borrowed: The object is owned by the caller and borrowed by the function call. After the call, the caller remains responsible for releasing the object at some point.
* @b borrowed-by @c user : The object is owned by the caller and borrowed by libfunnel object @c user. The caller must release @c user before it releases this object.
* @b owned: The object ownership is transferred to libfunnel. After the call, the caller must no longer use nor release the object.

For output arguments:

* @b borrowed-from @c parent : The object is owned by the parent object @c parent. Once ownership of @c parent is transferred back to libfunnel, the borrowed object becomes invalid and may no longer be used.
* @b owned: The object ownership is transferred to the caller. After the call, the caller must eventually release the object.
* @b owned-from: The object ownership is transferred to the caller, but it is a child object of @c parent. The object must be released by the caller before @c parent is released.

## API documentation

- @ref funnel.h "<funnel/funnel.h>" The core API used by all backends
- @ref funnel-gbm.h "<funnel/funnel-gbm.h>" Raw GBM buffer API
- @ref funnel-vk.h "<funnel/funnel-vk.h>" Vulkan API integration
- @ref funnel-egl.h "<funnel/funnel-egl.h>" EGL API integration
