#ifndef DBR_SFML_BITMAP_TEXT_HPP
#define DBR_SFML_BITMAP_TEXT_HPP

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <SFML/System/String.hpp>

namespace dbr
{
	namespace sfml
	{
		class BitmapFont;

		class BitmapText : public sf::Transformable, public sf::Drawable
		{
		public:
			BitmapText();
			BitmapText(const sf::String& str, const BitmapFont& fnt);

			void setString(const sf::String& str);
			void setFont(const BitmapFont& fnt);

			const sf::String& getString() const;
			const BitmapFont* getFont() const;

			sf::FloatRect getLocalBounds() const;
			sf::FloatRect getGlobalBounds() const;

		private:
			virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

			void update();

			static constexpr std::size_t TAB_WIDTH = 4u;	// in characters

			sf::String string;
			const BitmapFont* font;
			sf::VertexArray vertices;
		};
	}
}

#endif
