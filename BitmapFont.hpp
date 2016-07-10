#ifndef DBR_SFML_BITMAP_FONT_HPP
#define DBR_SFML_BITMAP_FONT_HPP

#include <array>
#include <vector>

#include <SFML/System/InputStream.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace dbr
{
	namespace sfml
	{
		/*
			Represents a bitmap font.
			Glyph indices should increment left and down, starting with Unicode 32 (space)
			If a codepoint is not needed, leave empty space for the codepoint in the image
			Max codepoint is limited by the max texture size of the graphics card
		*/
		class BitmapFont
		{
		public:
			BitmapFont() = default;
			~BitmapFont() = default;

			// calls corresponding texture.loadFrom*(...)
			// because of this, the memory provided by data/stream does not need to be preserved after this call
			bool loadFromFile(const std::string& filename, const sf::Vector2u& glyphSize, const sf::IntRect& area = sf::IntRect{});
			bool loadFromMemory(const void* data, std::size_t sizeInBytes, const sf::Vector2u& glyphSize, const sf::IntRect& area = sf::IntRect{});
			bool loadFromStream(sf::InputStream& stream, const sf::Vector2u& glyphSize, const sf::IntRect& area = sf::IntRect{});

			const sf::Texture& getTexture() const;
			const sf::Vector2u& getGlyphSize() const;

			// calls texture.setSmooth(s)
			void smooth(bool s);

			// returns the top-left texture coordinate of codePoint
			// if does not exist, returns {0, 0}
			sf::Vector2u getTextureCoord(sf::Uint32 codePoint) const;

		private:
			sf::Texture texture;
			sf::Vector2u glyphSize;
		};
	}
}

#endif
