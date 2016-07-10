#include "BitmapFont.hpp"

namespace dbr
{
	namespace sfml
	{
		bool BitmapFont::loadFromFile(const std::string& filename, const sf::Vector2u& glyphSize, const sf::IntRect& area)
		{
			this->glyphSize = glyphSize;
			return texture.loadFromFile(filename, area);
		}

		bool BitmapFont::loadFromMemory(const void* data, std::size_t sizeInBytes, const sf::Vector2u& glyphSize, const sf::IntRect& area)
		{
			this->glyphSize = glyphSize;
			return texture.loadFromMemory(data, sizeInBytes, area);
		}

		bool BitmapFont::loadFromStream(sf::InputStream& stream, const sf::Vector2u& glyphSize, const sf::IntRect& area)
		{
			this->glyphSize = glyphSize;
			return texture.loadFromStream(stream, area);
		}

		const sf::Texture& BitmapFont::getTexture() const
		{
			return texture;
		}

		const sf::Vector2u& BitmapFont::getGlyphSize() const
		{
			return glyphSize;
		}

		void BitmapFont::smooth(bool s)
		{
			texture.setSmooth(s);
		}

		sf::Vector2u BitmapFont::getTextureCoord(sf::Uint32 codePoint) const
		{
			const auto texSize = texture.getSize();
			const auto cols = texSize.x / glyphSize.x;
			const auto rows = texSize.y / glyphSize.y;

			// cancel out control characters
			codePoint -= 32;

			if(codePoint < cols * rows)
			{
				return{glyphSize.x * (codePoint % cols), glyphSize.y * (codePoint / cols)};
			}
			else
			{
				// requested a codePoint we don't have
				return{0, 0};
			}
		}
	}
}
