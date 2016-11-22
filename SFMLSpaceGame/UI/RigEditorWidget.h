#pragma once

class RigEditorWidget
{
public:
	virtual ~RigEditorWidget()
	{
	}

	virtual void Save() = 0;
	virtual void Delete() = 0;
	virtual sfg::Widget::Ptr GetWidget() = 0;
};