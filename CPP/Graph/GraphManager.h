#pragma once
#include"node_types.h"
#include"JSONParser.h"
typedef std::map<std::string, node_ptr> NodeTbl;
class CGraphInfo {
public:
	node_ptr m_root;
	NodeTbl m_nodeTbl;
};
typedef std::shared_ptr<CGraphInfo> graph_ptr;
class CGraphManager {
	std::stack<graph_ptr> m_graphStack;

	static std::shared_ptr<CGraphManager> s_Self;
public:
	static std::shared_ptr<CGraphManager> GetInstance();
	static void Release();

	void pushFromFile(std::string filePath);
	void popFromFile(std::string filePath);

	void pushGraph(json_node_ptr root);
	void popGraph(json_node_ptr root);
	void returnGraph() {
		if (!m_graphStack.empty()) {
			m_graphStack.pop();
		}
		if (!m_graphStack.empty()) {
			m_graphStack.top()->m_root->onActivate(true);
		}

	}
	node_ptr getRoot() {
		if (!m_graphStack.empty()) {
			return m_graphStack.top()->m_root;
		}

		return nullptr;
	}
	node_ptr getNode(std::string name) {
		if (!m_graphStack.empty()) {
			NodeTbl::iterator it=m_graphStack.top()->m_nodeTbl.find(name);
			if (it != m_graphStack.top()->m_nodeTbl.end()) {
				return it->second;
			}
		}
		return nullptr;

	}
	graph_ptr getRootGraph() {
		if (!m_graphStack.empty()) {
			return m_graphStack.top();
		}

		return nullptr;
	}
#ifdef USE_D2D
	virtual void Paint(image_render_target_ptr target, CDWTextFormat& f) ;

#endif

	void update();
private:
	graph_ptr createGraph(json_node_ptr p);
	node_ptr createNode(CGraphInfo& gi,json_node_ptr p);
	chara_ptr createChara(json_node_ptr p);
	void initArts(CCharactor& target, json_node_ptr p);

	void loadParserFromFile(CJSONParser& parser, std::string filePath);

};

