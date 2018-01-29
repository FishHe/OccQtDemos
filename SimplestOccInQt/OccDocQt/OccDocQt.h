#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_OccDocQt.h"

class DocumentCommon;
class View;

class OccDocQt : public QMainWindow
{
	Q_OBJECT

public:
	OccDocQt(QWidget *parent = Q_NULLPTR);

private:
	Ui::OccDocQtClass ui;


public:
	OccDocQt(DocumentCommon* aDocument, QWidget* parent, Qt::WindowFlags wflags);
	OccDocQt(View* aView, DocumentCommon* aDocument, QWidget* parent, Qt::WindowFlags wflags);
	~OccDocQt();

	DocumentCommon*            getDocument();
	void                       fitAll();
	virtual QSize              sizeHint() const;

signals:
	void                       selectionChanged();
	void                       message(const QString&, int);
	void                       sendCloseView(OccDocQt* theView);

	public slots:
	void                       closeEvent(QCloseEvent* e);
	void                       onWindowActivated();
	void                       dump();

protected:
	void                       createViewActions();
	void                       createRaytraceActions();

protected:
	DocumentCommon*            myDocument;
	View*                      myView;
};
