#pragma once

namespace UserInterface {
	class IElementUI{
	public:
		virtual void render() = 0;
		virtual void update() = 0;
		virtual bool checkClick() = 0;
	};
}