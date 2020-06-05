#pragma once
#include "KG_Core.h"
#include "KG_ShapeBox.h"
#include "KG_ModelViewCamera.h"
#include "KG_BackViewCamera.h"
#include "CBY_Object.h"

class main:public KG_Core
{
public:
	std::shared_ptr<KG_ShapeBox> m_Box;
	std::shared_ptr<KG_ModelViewCamera> m_ModelViewCamera;
	std::shared_ptr<KG_BackViewCamera> m_BackViewCamera;
	std::shared_ptr<KG_DebugCamera> m_DebugCamera;
	std::shared_ptr<CBY::CBY_Object> m_Object;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Relese();

public:
	main();
	virtual ~main();
};

KGCA37_RUN(main,xx,xx)

