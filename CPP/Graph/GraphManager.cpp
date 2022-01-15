#include"GraphManager.h"
#include"GraphError.h"
std::shared_ptr<CGraphManager> CGraphManager::s_Self;
std::shared_ptr<CGraphManager> CGraphManager::GetInstance() {
	if (nullptr == s_Self) {
		s_Self.reset(new CGraphManager());
	}
	return s_Self;
}
void CGraphManager::Release() {
	s_Self.reset();
}
void CGraphManager::pushFromFile(std::string filePath) {



	
	CJSONParser parser;
	loadParserFromFile(parser, filePath);
	pushGraph(parser.getRoot());
}
void CGraphManager::popFromFile(std::string filePath) {
	CJSONParser parser;
	loadParserFromFile(parser, filePath);
	popGraph(parser.getRoot());

}
void CGraphManager::pushGraph(json_node_ptr root) {
	if (!m_graphStack.empty()) {
		//m_graphStack.top()->m_root->onActivate(false);
		
	}
	m_graphStack.push(createGraph(root));
}
void CGraphManager::popGraph(json_node_ptr root) {
	if (!m_graphStack.empty()) {
		//m_graphStack.top()->m_root->onActivate(false);
		//m_graphStack.top()->m_nodeTbl.clear();
		m_graphStack.pop();
	}
	pushGraph(root);
}
void CGraphManager::Paint(image_render_target_ptr target, CDWTextFormat& f) {
	if (nullptr != getRoot()) {
		getRoot()->Paint(target, f);
	}

}
void CGraphManager::update() {
	if (nullptr != getRoot()) {
		getRoot()->update();
	}

}
graph_ptr CGraphManager::createGraph(json_node_ptr p) {
	graph_ptr r(new CGraphInfo());
	r->m_root = createNode(*r.get(),p);
	return r;

}
node_ptr CGraphManager::createNode(CGraphInfo& gi, json_node_ptr p) {
	
	CJSONChunck* lpChunck = dynamic_cast<CJSONChunck*>(p.get());
	if (nullptr != lpChunck) {
		json_node_ptr tmp = lpChunck->getElement("type");
		if (nullptr != tmp) {
			CJSONString* lpType = tmp->castTo<CJSONString>();
			if ("screen" == lpType->value()) {
				CScreenNode* lpScreen = new CScreenNode();
				node_ptr r(lpScreen);
				tmp = lpChunck->getElement("width");
				CJSONInt* lpWidth = nullptr;
				if (nullptr != tmp) {
					lpWidth = tmp->castTo<CJSONInt>();
				}
				tmp = lpChunck->getElement("height");
				CJSONInt* lpHeight = nullptr;
				if (nullptr != tmp) {
					lpHeight = tmp->castTo<CJSONInt>();
				}
				lpScreen->size() = CSize(
					nullptr != lpWidth ? lpWidth->value() : 0,
					nullptr != lpHeight ? lpHeight->value() : 0);

				tmp = lpChunck->getElement("children");
				if (nullptr != tmp) {
					CJSONArray* lpChildren = tmp->castTo<CJSONArray>();
					for (json_node_ptr c : lpChildren->value()) {
						node_ptr child = createNode(gi,c);
						if (!child->name().empty()) {
							gi.m_nodeTbl.insert(NodeTbl::value_type(child->name(), child));
						}
						lpScreen->children().push_back(child);
					}
					
				}
				tmp = lpChunck->getElement("charactors");
				if (nullptr != tmp) {
					CJSONArray* lpChildren = tmp->castTo<CJSONArray>();
					for (json_node_ptr c : lpChildren->value()) {
						chara_ptr cc = createChara(c);
						CCharaManager::s_Self->add(cc);
					}
				}

				return r;
			}
			else if ("rouge" == lpType->value()) {
				CRougeNode* lpRouge = new CRougeNode();
				node_ptr r(lpRouge);

				tmp = lpChunck->getElement("left");
				CJSONInt* lpLeft = nullptr;
				if (nullptr != tmp) {
					lpLeft = tmp->castTo<CJSONInt>();
				}
				tmp = lpChunck->getElement("top");
				CJSONInt* lpTop = nullptr;
				if (nullptr != tmp) {
					lpTop = tmp->castTo<CJSONInt>();
				}
				tmp = lpChunck->getElement("width");
				CJSONInt* lpWidth = nullptr;
				if (nullptr != tmp) {
					lpWidth = tmp->castTo<CJSONInt>();
				}
				tmp = lpChunck->getElement("height");
				CJSONInt* lpHeight = nullptr;
				if (nullptr != tmp) {
					lpHeight = tmp->castTo<CJSONInt>();
				}
				lpRouge->border() = CRect(
					nullptr != lpLeft ? lpLeft->value() : 0,
					nullptr != lpTop ? lpTop->value() : 0,
					nullptr != lpWidth ? lpWidth->value() : 0,
					nullptr != lpHeight ? lpHeight->value() : 0);
				

					std::string pcName = castTo<std::string>(lpChunck->getElement("pc"),"");
					if (!pcName.empty()) {
						lpRouge->setPCChara(CCharaManager::s_Self->getChara(pcName)->clone());
					}
					lpRouge->reset();
				return r;

			}
			
			else if ("int_var" == lpType->value()) {
				IntVarNode* lpIntVar = new IntVarNode();
				node_ptr r(lpIntVar);
				r->name() = castTo<std::string>(lpChunck->getElement("name"), "");
				lpIntVar->init() = castTo<int>(lpChunck->getElement("init"), 0);
				lpIntVar->reset();
				return r;

			}
			else if ("counter" == lpType->value()) {
				CCounterNode* lpCounter = new CCounterNode();
				node_ptr r(lpCounter);
				r->name() = castTo<std::string>(lpChunck->getElement("name"), "");
				lpCounter->target() = castTo < std::string>(lpChunck->getElement("target"), "");
				lpCounter->incValue() = castTo < int>(lpChunck->getElement("inc"), 0);
				return r;
			}
			else if ("image" == lpType->value()) {
				CImageNode* lpImage = new CImageNode();
				node_ptr r(lpImage);
				r->name() = castTo<std::string>(lpChunck->getElement("name"), "");
				lpImage->border() = CRect(
					castTo<int>(lpChunck->getElement("left"), 0),
					castTo<int>(lpChunck->getElement("top"), 0),
					castTo<int>(lpChunck->getElement("width"), 0),
					castTo<int>(lpChunck->getElement("height"), 0)

				);
				lpImage->imageSrc()= castTo<std::string>(lpChunck->getElement("src"), "");
				lpImage->onClick() = castTo<std::string>(lpChunck->getElement("onClick"), "");
				return r;
			}
			else if ("text" == lpType->value()) {
				CTextNode* lpText = new CTextNode();
				node_ptr r(lpText);
				r->name() = castTo<std::string>(lpChunck->getElement("name"), "");
				lpText->border() = CRect(
					castTo<int>(lpChunck->getElement("left"), 0),
					castTo<int>(lpChunck->getElement("top"), 0),
					castTo<int>(lpChunck->getElement("width"), 0),
					castTo<int>(lpChunck->getElement("height"), 0)

				);
				std::string caption = castTo<std::string>(lpChunck->getElement("caption"), "");
				lpText->setCaption( utf8_to_wide(caption));
				lpText->onClick() = castTo<std::string>(lpChunck->getElement("onClick"), "");
				return r;

			
			}
			else if ("next_scene" == lpType->value()) {
				CNextSceneNode* lpNextScene = new CNextSceneNode();
				node_ptr r(lpNextScene);
				r->name() = castTo<std::string>(lpChunck->getElement("name"), "");
				gi.m_nodeTbl.insert(NodeTbl::value_type(r->name(), r));

				std::string moveType = castTo<std::string>(lpChunck->getElement("move_type"), "pop");
				if ("push" == moveType) {
					lpNextScene->moveType() = CNextSceneNode::MOVE_TYPE::PUSH;
				}else if ("pop" == moveType) {
					lpNextScene->moveType() = CNextSceneNode::MOVE_TYPE::POP;
				}else if ("return" == moveType) {
					lpNextScene->moveType() = CNextSceneNode::MOVE_TYPE::RETURN;
				}

				std::string target = castTo<std::string>(lpChunck->getElement("target"), "");
				lpNextScene->setTarget(target);

				return r;

			}
		}
	}

	return nullptr;
}
chara_ptr CGraphManager::createChara(json_node_ptr p) {
	CJSONChunck* lpChunck = dynamic_cast<CJSONChunck*>(p.get());
	if (nullptr != lpChunck) {
		CCharactor* lpChara = new CCharactor();
		chara_ptr r(lpChara);
		std::string name=castTo < std::string>(lpChunck->getElement("name"), "");
		lpChara->setName(name);

		bool isPlayerUseOnly = castTo<bool>(lpChunck->getElement("player_user_only"), false);
		lpChara->flags() |= (isPlayerUseOnly ? CCharactor::FLAG::PLAYER_USE_ONLY : 0);
		bool isFieldObject= castTo<bool>(lpChunck->getElement("field_object"), false);
		lpChara->flags() |= (isFieldObject ? CCharactor::FLAG::FIELD_OBJECT : 0);

		int life= castTo<int>(lpChunck->getElement("life"), 0);
		lpChara->getCurrentStatus().life = life;
		int atk = castTo<int>(lpChunck->getElement("atk"), 0);
		lpChara->getCurrentStatus().atk = atk;

		json_node_ptr artE = lpChunck->getElement("arts");
		initArts(*lpChara, artE);
		return r;
	
	}
	return nullptr;

}
void CGraphManager::initArts(CCharactor& target, json_node_ptr p) {
	if (nullptr == p) {
		return;
	}

	CJSONChunck* lpChunck = dynamic_cast<CJSONChunck*>(p.get());
	std::string artsType= castTo < std::string>(lpChunck->getElement("type"), "asteroid");
	if ("asteroid" == artsType) {
		target.arts().type() = CArts::TYPE::ASTEROID;
	}
	else if ("hound" == artsType) {
		target.arts().type() = CArts::TYPE::HOUND;

	}
	int range=castTo<int>(lpChunck->getElement("range"), 0);
	target.arts().range() = range;
	int rad = castTo<int>(lpChunck->getElement("rad"), 0);
	target.arts().rad() = rad;
}
void CGraphManager::loadParserFromFile(CJSONParser& parser, std::string filePath) {
	std::fstream f(filePath);
	std::string src;
	std::string line;
	do {
		std::getline(f, line);
		src += line + "\n";

	} while (!f.eof());
	parser.parse(src);


}