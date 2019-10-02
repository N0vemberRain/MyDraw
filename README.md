# MyDraw

Темы.

1. 
Widget: background-color: rgb(50, 50, 50);\ncolor: white;\nflat:  px10;
Label: color: white; font: 14px;
ComboBox: QComboBox {
	background-color: white;
    border: 1px solid gray;
    border-radius: 3px;
    padding: 1px 10px 1px 3px;
    max-width: 6em;
	color: black;
	font: 14px;
}

/* Background color of popup-list.*/ 
QComboBox QListView{
    background-color:white;
    border:1px solid grey;
}
/* Needed to complete the rule set. */
QComboBox::item:alternate {
    background: rgb(71, 73, 73);
}
LineEdit: background-color: white;
color: black;
font: 14px;
PushBotton: background-color: white;\ncolor: black;\nfont:  14px;
///////////////////////////////////////////////////////////
TabWidget: QTabWidget { 
	 background: rgb(30,30,30);
	color: white; 
}

QTabBar::tab {
	 background: rgb(15,15,15); 
	color: white;
}

QTabBar::tab:selected { 
	background: rgb(50,50,50); 
	color: white;
}

QTabWidget::pane { /* The tab widget frame */
    border-style: out;
}

QTabWidget { border-style: out; }
