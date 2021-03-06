// @date 2016/08/10
// @author Mao Jingkai(oammix@gmail.com)

#pragma once

#include <resource/resource.hpp>
#include <graphics/graphics.hpp>
#include <math/color.hpp>

NS_LEMON_RESOURCE_BEGIN

enum class ImageElementFormat : uint8_t
{
    UBYTE = 0,
    USHORT_565,
    USHORT_4444,
    USHORT_5551
};

struct Image : public Resource
{
    using ptr = std::shared_ptr<Image>;
    using weak_ptr = std::weak_ptr<Image>;

    virtual ~Image();

    bool read(std::istream&) override;
    bool save(std::ostream&) override;
    bool update_video_object() override;

    size_t get_memory_usage() const override;
    size_t get_video_memory_usage() const override;

    // set size and number of color components, old pixels will be discarded.
    bool initialize(unsigned, unsigned, unsigned, ImageElementFormat element = ImageElementFormat::UBYTE);
    // set new image data
    void set_data(const uint8_t* data);
    void set_data_raw(const uint8_t* data, unsigned offset, unsigned size);
    // clear the image with a color
    void clear(const math::Color&);
    // set a 2d pixel
    void set_pixel(unsigned, unsigned, const math::Color&);
    // specifies the expected usage pattern of the data source
    void set_video_memory_hint(graphics::BufferUsage);
    // return a 2D pixel color
    math::Color get_pixel(unsigned, unsigned) const;
    // return a linearly sampled 2d pixel color, x and y have the range 0-1
    math::Color get_pixel_linear(float, float) const;
    // return the raw memory pointer of image
    const void* get_data() const { return _data.get(); }
    // return the width/height/components of image
    unsigned get_width() const { return _width; }
    unsigned get_height() const { return _height; }
    unsigned get_components() const { return _components; }
    // returns the internal format of image
    ImageElementFormat get_element_format() const { return _element_format; }
    // returns memory usage of this image
    graphics::BufferUsage get_video_memory_hint() const { return _usage; }
    // returns graphics object of texture
    Handle get_video_uid() const;

protected:
    // with/height
    unsigned _width = 0, _height = 0, _components = 1;
    // pixel data
    std::unique_ptr<uint8_t[]> _data;
    //
    ImageElementFormat _element_format = ImageElementFormat::UBYTE;
    //
    graphics::BufferUsage _usage = graphics::BufferUsage::STATIC;
    // 
    Handle _video_uid;
};

INLINE void Image::set_video_memory_hint(graphics::BufferUsage usage)
{
    _usage = usage;
}

INLINE Handle Image::get_video_uid() const
{
    return _video_uid;
}

// INLINE graphics::Texture* Image::get_texture() const
// {
//     return _texture;
// }

NS_LEMON_RESOURCE_END