#include "BitmapText.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "BitmapFont.hpp"

namespace dbr
{
	namespace sfml
	{
		BitmapText::BitmapText()
			: font(nullptr),
			vertices(sf::Quads)
		{}

		BitmapText::BitmapText(const sf::String& str, const BitmapFont& fnt)
			: string(str),
			font(&fnt),
			vertices(sf::Quads, str.getSize() * 4)	// 4 vertices per character
		{
			update();
		}

		void BitmapText::setString(const sf::String& str)
		{
			string = str;
			update();
		}

		void BitmapText::setFont(const BitmapFont& fnt)
		{
			font = &fnt;
			update();
		}

		const sf::String& BitmapText::getString() const
		{
			return string;
		}

		const BitmapFont* BitmapText::getFont() const
		{
			return font;
		}

		sf::FloatRect BitmapText::getLocalBounds() const
		{
			if(font == nullptr)
				return{};

			std::size_t idx = 0;
			std::size_t numLines = 0;
			std::size_t longestLine = 0;

			const auto strSize = string.getSize();

			while(idx != sf::String::InvalidPos && idx < strSize)
			{
				std::size_t next = std::min(string.find('\n', idx), strSize);

				auto tabs = std::count(string.begin() + idx, string.begin() + next, '\t');

				auto len = next - idx + tabs * (TAB_WIDTH - 1);	// "next - idx" already counts each tab once

				longestLine = std::max(longestLine, len);

				idx = next + 1;
				++numLines;
			}

			auto size = font->getGlyphSize();

			return{0, 0, static_cast<float>(longestLine * size.x), static_cast<float>(numLines * size.y)};
		}

		sf::FloatRect BitmapText::getGlobalBounds() const
		{
			return getTransform().transformRect(getLocalBounds());
		}

		void BitmapText::draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			if(font)
			{
				states.transform *= getTransform();
				states.texture = &font->getTexture();

				target.draw(vertices, states);
			}
		}

		void BitmapText::update()
		{
			if(font == nullptr)
				return;

			vertices.clear();

			// count number of spacing characters to remove from total string length,
			// for pre-calculating number of vertices needed
			const auto spaceChars = std::count_if(string.begin(), string.end(), [](auto c) { return c == ' ' || c == '\n' || c == '\t'; });

			const auto drawableChars = string.getSize() - spaceChars;

			vertices.resize(drawableChars * 4);	// 4 verts per character

			const auto glyphSize = static_cast<sf::Vector2f>(font->getGlyphSize());
			sf::Vector2f pos;

			auto vertIdx = 0u;
			for(auto i = 0u; i < string.getSize(); ++i, ++vertIdx)
			{
				auto c = string[i];

				switch(c)
				{
					case ' ':
						pos.x += glyphSize.x;
						goto NoDrawable;

					case '\t':
						pos.x += glyphSize.x * TAB_WIDTH;
						goto NoDrawable;

					case '\n':
						pos.x = 0;
						pos.y += glyphSize.y;
						goto NoDrawable;

					// we didn't add a drawable character
NoDrawable:
					case -1:
						--vertIdx;
						continue;

					default:
					{
						auto* vert = &vertices[vertIdx * 4];

						auto texCoord = static_cast<sf::Vector2f>(font->getTextureCoord(c));

						// top left
						vert->color = sf::Color{0xffffffff};
						vert->position = pos;
						vert->texCoords = texCoord;
						++vert;

						// top right
						vert->color = sf::Color{0xffffffff};
						vert->position = pos + sf::Vector2f{glyphSize.x, 0};
						vert->texCoords = texCoord + sf::Vector2f{glyphSize.x, 0};
						++vert;

						// bot right
						vert->color = sf::Color{0xffffffff};
						vert->position = pos + glyphSize;
						vert->texCoords = texCoord + glyphSize;
						++vert;

						// bot left
						vert->color = sf::Color{0xffffffff};
						vert->position = pos + sf::Vector2f{0, glyphSize.y};
						vert->texCoords = texCoord + sf::Vector2f{0, glyphSize.y};

						// next character
						pos.x += glyphSize.x;

						break;
					}
				}
			}
		}
	}
}
