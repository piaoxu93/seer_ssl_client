import QtQuick 2.6
import QtQuick.Controls 1.5
import QtQuick.Controls.Styles 1.4
import Client.Component 1.0 as Client
ApplicationWindow{
    visible:true;
    width:1350;
    height:700;
    minimumHeight: height;
    minimumWidth: width;
    maximumHeight: height;
    maximumWidth: width;
    color:"#FFFFFF";
    id:window;
    Client.CommandParser{ id: commandParser; }
    Client.Serial { id : serial; }
    Timer{
        id:timer;
        interval:15;
        running:false;
        repeat:true;
        onTriggered: {
            serial.sendCommand();
        }
    }
    Grid{
        rows:2;
        columns:2;
        rowSpacing:0;
        columnSpacing:0;
        Client.Field{
            width:1050;
            height:700;
            id:fieldCanvas;
            MouseArea{
                anchors.fill: parent;
                hoverEnabled : true;
                onPositionChanged:{
                    mousePos.text = "( " + Math.floor(mouseX)  + " , " + Math.floor(mouseY) + " )";
                }
            }
            Text{
                id : mousePos;
                text : "";
            }
        }
        TabView{
            style: TabViewStyle {
                frameOverlap: 1
                tab: Rectangle {
                    color: styleData.selected ? "lightgrey" : "white"
                    implicitWidth: operationPanel.width/operationPanel.count;
                    implicitHeight: 20
                    radius: 2
                    Text {
                        id: text
                        anchors.centerIn: parent
                        text: styleData.title
                        color: styleData.selected ? "black" : "grey"
                    }
                }
                frame: Rectangle { color: "lightgrey" }
            }
            width:window.width - fieldCanvas.width;
            height:fieldCanvas.height;
            id: operationPanel;
            Tab{
                anchors.fill: parent;
                title: "Radio";
                Rectangle{
                    width:parent.width;
                    anchors.top: parent.top;
                    anchors.bottom: parent.bottom;
                    color : "lightgrey";
                    Rectangle{
                        id : crazyListRectangle;
                        width: parent.width;
                        color : "lightgrey";
                        height: 240;
                        ListView{
                            id : crazyListView;
                            delegate:crazyComponent;
                            anchors.fill: parent;
                            anchors.margins: 20;
                            spacing: 10;
                            model: 6;
                            enabled: !crazyConnect.ifConnected;
                        }
                    }
                    Button{
                        id : crazyConnect;
                        text: "Connect";
                        property bool ifConnected:false;
                        anchors.top: crazyListRectangle.bottom;
                        anchors.right: parent.right;
                        anchors.rightMargin: 20;
                        onClicked: {
                            if(ifConnected){
                                timer.stop();
                                if(crazyStart.ifStarted) crazyStart.handleClickEvent();
                                serial.closeSerialPort();
                            }else{
                                serial.openSerialPort();
                                serial.sendStartPacket();
                            }
                            ifConnected = !ifConnected;
                            text = ifConnected ? "Disconnect" : "Connect";
                        }
                    }
                    Grid{
                        id : crazyShow;
                        anchors.top:crazyConnect.bottom;
                        anchors.horizontalCenter: parent.horizontalCenter;
                        anchors.margins: 20;
                        columns: 4;
                        rows:5;
                        verticalItemAlignment: Grid.AlignVCenter;
                        horizontalItemAlignment: Grid.AlignLeft;
                        columnSpacing: 10;
                        rowSpacing: 5;
                        focus: true;
                        property int m_VEL : 255
                        property int m_VELR : 1023
                        property int velX : 0;
                        property int velY : 0;
                        property int velR : 0;
                        property bool shoot : false;
                        property bool dribble : false;
                        property int power : 127;
                        property int velStep : 1;
                        property bool mode : false;
                        property int robotID : 0;
                        property int itemWidth : 70;
                        Text{ text:"robot  " }
                        SpinBox{ minimumValue:0; maximumValue:11; value:parent.robotID; width:parent.itemWidth
                            onEditingFinished:{parent.robotID = value}}
                        Text{ text:" " }
                        Text{ text:" " }
                        Text{ text:"vel-x" }
                        SpinBox{ minimumValue:-crazyShow.m_VEL; maximumValue:crazyShow.m_VEL; value:parent.velX;width:parent.itemWidth
                            onEditingFinished:{parent.velX = value;}}
                        Text{ text:"dribb" }
                        Button{ text:parent.dribble;width:parent.itemWidth
                            onClicked: {
                                parent.dribble = !parent.dribble;
                                serial.updateCommandParams(crazyShow.robotID,crazyShow.velX,crazyShow.velY,crazyShow.velR,crazyShow.dribble,crazyShow.mode,crazyShow.shoot,crazyShow.power);
                            }
                        }
                        Text{ text:"vel-y "}
                        SpinBox{ minimumValue:-crazyShow.m_VEL; maximumValue:crazyShow.m_VEL; value:parent.velY;width:parent.itemWidth
                            onEditingFinished:{parent.velY = value;}}
                        Text{ text:"shoot"  }
                        Button{ text:parent.shoot;width:parent.itemWidth
                            onClicked: {
                                parent.shoot = !parent.shoot;
                                serial.updateCommandParams(crazyShow.robotID,crazyShow.velX,crazyShow.velY,crazyShow.velR,crazyShow.dribble,crazyShow.mode,crazyShow.shoot,crazyShow.power);
                            }
                        }
                        Text{ text:"vel-r"  }
                        SpinBox{ minimumValue:-crazyShow.m_VELR; maximumValue:crazyShow.m_VELR; value:parent.velR;width:parent.itemWidth
                            onEditingFinished:{parent.velR = value;}}
                        Text{ text:"mode"  }
                        Button{ text:(parent.mode?"lift":"flat");width:parent.itemWidth
                            onClicked: {
                                parent.mode = !parent.mode
                                serial.updateCommandParams(crazyShow.robotID,crazyShow.velX,crazyShow.velY,crazyShow.velR,crazyShow.dribble,crazyShow.mode,crazyShow.shoot,crazyShow.power);
                            }
                        }
                        Text{ text:"step" }
                        SpinBox{ minimumValue:1; maximumValue:crazyShow.m_VEL; value:parent.velStep;width:parent.itemWidth
                            onEditingFinished:{parent.velStep = value;}}
                        Text{ text:"power"  }
                        SpinBox{ minimumValue:0; maximumValue:127; value:parent.power;width:parent.itemWidth
                            onEditingFinished:{parent.power = value;}}
                        Keys.onPressed:getFocus(event);
                        function getFocus(event){
                            switch(event.key){
                            case Qt.Key_Enter:
                            case Qt.Key_Return:
                            case Qt.Key_Escape:
                                crazyShow.focus = true;
                                break;
                            default:
                                event.accepted = false;
                                return false;
                            }
                            event.accepted = true;
                        }
                        function handleKeyboardEvent(e){
                            switch(e){
                            case 'U':{crazyShow.mode = !crazyShow.mode;break;}
                            case 'a':{crazyShow.velY = crazyShow.limitVel(crazyShow.velY-crazyShow.velStep,-crazyShow.m_VEL,crazyShow.m_VEL);
                                break;}
                            case 'd':{crazyShow.velY = crazyShow.limitVel(crazyShow.velY+crazyShow.velStep,-crazyShow.m_VEL,crazyShow.m_VEL);
                                break;}
                            case 'w':{crazyShow.velX = crazyShow.limitVel(crazyShow.velX+crazyShow.velStep,-crazyShow.m_VEL,crazyShow.m_VEL);
                                break;}
                            case 's':{crazyShow.velX = crazyShow.limitVel(crazyShow.velX-crazyShow.velStep,-crazyShow.m_VEL,crazyShow.m_VEL);
                                break;}
                            case 'q':{crazyShow.dribble = !crazyShow.dribble;
                                break;}
                            case 'e':{crazyShow.shoot = !crazyShow.shoot;
                                break;}
                            case 'L':{crazyShow.velR = crazyShow.limitVel(crazyShow.velR+crazyShow.velStep,-crazyShow.m_VELR,crazyShow.m_VELR);
                                break;}
                            case 'R':{crazyShow.velR = crazyShow.limitVel(crazyShow.velR-crazyShow.velStep,-crazyShow.m_VELR,crazyShow.m_VELR);
                                break;}
                            case 'S':{crazyShow.velX = 0;
                                    crazyShow.velY = 0;
                                    crazyShow.velR = 0;
                                    crazyShow.shoot = false;
                                    crazyShow.dribble = false;
                                break;}
                            default:
                                return false;
                            }
                            serial.updateCommandParams(crazyShow.robotID,crazyShow.velX,crazyShow.velY,crazyShow.velR,crazyShow.dribble,crazyShow.mode,crazyShow.shoot,crazyShow.power);
                        }
                        function limitVel(vel,minValue,maxValue){
                            if(vel>maxValue) return maxValue;
                            if(vel<minValue) return minValue;
                            return vel;
                        }
                        Shortcut{
                            sequence:"A";
                            onActivated:crazyShow.handleKeyboardEvent('a');
                        }
                        Shortcut{
                            sequence:"Up";
                            onActivated:crazyShow.handleKeyboardEvent('U');
                        }
                        Shortcut{
                            sequence:"D"
                            onActivated:crazyShow.handleKeyboardEvent('d');
                        }
                        Shortcut{
                            sequence:"W"
                            onActivated:crazyShow.handleKeyboardEvent('w');
                        }
                        Shortcut{
                            sequence:"S"
                            onActivated:crazyShow.handleKeyboardEvent('s');
                        }
                        Shortcut{
                            sequence:"Q"
                            onActivated:crazyShow.handleKeyboardEvent('q');
                        }
                        Shortcut{
                            sequence:"E"
                            onActivated:crazyShow.handleKeyboardEvent('e');
                        }
                        Shortcut{
                            sequence:"Left"
                            onActivated:crazyShow.handleKeyboardEvent('L');
                        }
                        Shortcut{
                            sequence:"Right"
                            onActivated:crazyShow.handleKeyboardEvent('R');
                        }
                        Shortcut{
                            sequence:"Space"
                            onActivated:crazyShow.handleKeyboardEvent('S');
                        }
                    }
                    Button{
                        id : crazyStart;
                        text:"Start";
                        property bool ifStarted:false;
                        anchors.right:parent.right;
                        anchors.rightMargin: 20;
                        anchors.top:crazyShow.bottom;
                        anchors.topMargin: 20;
                        enabled : crazyConnect.ifConnected;
                        onClicked:{
                            handleClickEvent();
                        }
                        function handleClickEvent(){
                            if(ifStarted){
                                timer.stop();
                            }else{
                                timer.start();
                            }
                            ifStarted = !ifStarted;
                            text = ifStarted ? "Stop " : "Start";
                        }
                    }
                }

            }
            Tab{
                anchors.fill: parent;
                property string title: "Referee";
                RefereeBox{}
            }
            Tab{
                anchors.fill: parent;
                property string title: "Vision";
                Vision{}
            }
        }
        Canvas{
            width:fieldCanvas.width;
            height:window.height - fieldCanvas.height;
            id:monitorCanvas;
        }
        Rectangle{

            width:window.width - fieldCanvas.width;
            height:window.height - fieldCanvas.height;
            id:terminal;
            color:"lightGreen";
            Rectangle{
                width:parent.width;
                height:parent.height > 100 ? 30 : 0;
                anchors.bottom:parent.bottom;
                TextField{
                    id:terminalInput;
                    anchors.fill:parent;
                    placeholderText: qsTr("Enter Command");
                    style: TextFieldStyle {
                        textColor: "black";
                        background: Rectangle {
                            radius: 2;
                            border.color: terminal.color;
                            border.width: 2;
                        }
                    }
                    Keys.onReturnPressed: enter(event);
                    Keys.onEnterPressed: enter(event);
                    function enter(event){
                        var text = terminalInput.text;
                        if(text != ""){
                            commandParser.sendCommand(text);
                            terminalInput.text = "";
                        }
                    }
                }
            }
        }
    }
    Component{
        id:crazyComponent;
        Item{
            width:parent.width;
            height:25;
            property int itemIndex: index;
            Text{
                anchors.left:parent.left;
                anchors.verticalCenter: parent.verticalCenter;
                anchors.leftMargin: 10;
                height:parent.height;
                text: serial.getName(itemIndex);
                lineHeight: parent.height;
            }
            ComboBox{
                anchors.right:parent.right;
                width:parent.width*0.5;
                height:parent.height;
                model:serial.getCrazySetting(itemIndex);
                currentIndex : serial.getDefaultIndex(itemIndex);
                onActivated: serial.sendCrazySetting(itemIndex,index);
            }
        }
    }
}


