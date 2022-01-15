#pragma once
#include"AssetsFramework.h"
class CADV {


	image_render_target_ptr m_target;
	typedef std::vector<std::wstring> TextTbl;
	TextTbl m_textTbl;

	image_ptr m_images[4];

public:
	CADV(image_render_target_ptr target)
		:m_target(target)
	{}


	void addText(std::wstring t) {
		m_textTbl.push_back(t);
	}

	void loadImage(int idx, std::string filePath) {
		m_images[idx].reset(new CD2DBitmapEx(filePath));
	}

	void clear(int idx);
};
