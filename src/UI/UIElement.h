#pragma once

namespace UserInterface {
	class UIElement {
	public:
		virtual void render() = 0;
		virtual void update() = 0;
		virtual bool checkClick() = 0;
	};
}