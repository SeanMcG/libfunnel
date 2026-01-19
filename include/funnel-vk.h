#pragma once

#include "funnel.h"
#include <vulkan/vulkan.h>

/**
 * Set up a stream for Vulkan integration.
 *
 * Required device extensions for Vulkan 1.2+:
 * - VK_KHR_external_semaphore_fd
 * - VK_KHR_external_memory_fd
 * - VK_EXT_external_memory_dma_buf
 * - VK_EXT_image_drm_format_modifier
 *
 * Additional required device extensions for Vulkan 1.1:
 * - VK_KHR_image_format_list
 *
 * Additional required device extensions for Vulkan 1.0:
 * - VK_KHR_external_memory
 * - VK_KHR_maintenance1
 * - VK_KHR_bind_memory2
 * - VK_KHR_sampler_ycbcr_conversion
 * - VK_KHR_get_memory_requirements2
 * - VK_KHR_external_semaphore
 *
 * Additional required instance extensions for Vulkan 1.0:
 * - VK_KHR_get_physical_device_properties2
 * - VK_KHR_external_memory_capabilities
 * - VK_KHR_external_semaphore_capabilities
 *
 * @param stream Stream
 * @param instance VkInstance to use for the stream (must outlive stream)
 * @param physical_device VkPhysicalDevice to use for the stream (must outlive
 * stream)
 * @param device VkDevice to use for the stream (must outlive stream)
 */
int funnel_stream_init_vulkan(struct funnel_stream *stream, VkInstance instance,
                              VkPhysicalDevice physical_device,
                              VkDevice device);

/**
 * Set the required buffer usage. This will control the usage for
 * images allocated by libfunnel.
 *
 * funnel_stream_vk_add_format() will fail if the requested usages
 * are not available. In this case, you may reconfigure the usage
 * and try again.
 *
 * @param stream Stream
 * @param usage Required VkImageUsageFlagBits.
 */
int funnel_stream_vk_set_usage(struct funnel_stream *stream,
                               VkImageUsageFlagBits usage);

/**
 * Add a supported Vulkan format. Must be called in preference order (highest to
 * lowest). Only some formats are supported by libfunnel:
 *
 * - VK_FORMAT_R8G8B8A8_SRGB
 * - VK_FORMAT_R8G8B8A8_UNORM
 * - VK_FORMAT_B8G8R8A8_SRGB
 * - VK_FORMAT_B8G8R8A8_UNORM
 *
 * The corresponding UNORM variants are also acceptable, and equivalent.
 * `funnel_buffer_get_vk_format` will always return the SRGB formats. If
 * you need UNORM (because you are doing sRGB/gamma conversion in your shader),
 * you can use UNORM constants when you create a VkImageView.
 *
 * @param stream Stream
 * @param format VkFormat
 * @param alpha Whether alpha is meaningful or ignored
 * @param features Required VkFormatFeatureFlagBits. Adding a format will fail
 * if the requested features are not available.
 * @param usage Required VkImageUsageFlagBits. Adding a format will fail
 * if the requested usages are not available.
 */
int funnel_stream_vk_add_format(struct funnel_stream *stream, VkFormat format,
                                bool alpha, VkFormatFeatureFlagBits features);

/**
 * Get the VkImage for a Funnel buffer.
 *
 * The VkImage is only valid while `buf` is dequeued, or before the destroy
 * callback is used (if you use buffer callbacks).
 *
 * @param buf Buffer
 * @param bo Output VkImage for the buffer (borrowed)
 */
int funnel_buffer_get_vk_image(struct funnel_buffer *buf, VkImage *image);

/**
 * Get the VkFormat for a Funnel buffer.
 *
 * @param buf Buffer
 * @param format Output EGL format
 * @param has_alpha Output boolean indicating whether alpha is enabled
 */
int funnel_buffer_get_vk_format(struct funnel_buffer *buf, VkFormat *format,
                                bool *has_alpha);

/**
 * Get the VkSemaphores for acquiring and releasing the buffer.
 *
 * The user must wait on the acquier VkSemaphore object before accessing
 * the buffer, and signal the release VkSemaphore after accessing the buffer.
 * These semaphores are valid while the buffer is dequeued.
 *
 * @param buf Buffer
 * @param acquire Output acquire VkSemaphore (valid while the buffer is
 *                dequeued)
 * @param release Output release VkSemaphore (valid while the buffer is
 *                dequeued)
 */
int funnel_buffer_get_vk_semaphores(struct funnel_buffer *buf,
                                    VkSemaphore *acquire, VkSemaphore *release);
