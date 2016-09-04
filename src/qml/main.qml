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
    Client.Interaction{ id : interaction; }
    Client.Translator{ id : translator; }
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
        function fieldChangeSignal(ifBig){
            fieldCanvas.ifBig = ifBig;
        }
        Client.Field{
            width:1050;
            height:700;
            id:fieldCanvas;
            property bool ifBig : true;
            property int bigWidth : 1050;
            property int bigHeight: 700;
            property int smallWidth : 700;
            property int smallHeight : 467;
            MouseArea{
                anchors.fill: parent;
                hoverEnabled : true;
                onPositionChanged:{
                    var w = fieldCanvas.ifBig ? fieldCanvas.bigWidth : fieldCanvas.smallWidth;
                    var h = fieldCanvas.ifBig ? fieldCanvas.bigHeight : fieldCanvas.smallHeight;
                    var x = Math.floor((1/fieldCanvas.width*mouseX - 0.5)*w);
                    var y = -Math.floor((1/fieldCanvas.height*mouseY- 0.5)*h);
                    mousePos.text = "( " + x  + " , " + y + " )";
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
                title: qsTr("Radio") + translator.emptyString;
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
                            model: [qsTr("Ports")+ translator.emptyString,qsTr("BaudRate")+ translator.emptyString
                                ,qsTr("DataBits")+ translator.emptyString,qsTr("Parity")+ translator.emptyString
                                ,qsTr("StopBits")+ translator.emptyString,qsTr("Frequency")+ translator.emptyString];
                            enabled: !crazyConnect.ifConnected;
                        }
                    }
                    Button{
                        id : crazyConnect;
                        text: qsTr("Connect") + translator.emptyString;
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
                            text = (ifConnected ? qsTr("Disconnect") : qsTr("Connect")) + translator.emptyString;
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
                        Text{ text:qsTr("robot") + translator.emptyString }
                        SpinBox{ minimumValue:0; maximumValue:11; value:parent.robotID; width:parent.itemWidth
                            onEditingFinished:{parent.robotID = value}}
                        Text{ text:" " }
                        Text{ text:" " }
                        Text{ text:qsTr("vel-x") + translator.emptyString }
                        SpinBox{ minimumValue:-crazyShow.m_VEL; maximumValue:crazyShow.m_VEL; value:parent.velX;width:parent.itemWidth
                            onEditingFinished:{parent.velX = value;}}
                        Text{ text:qsTr("dribb") + translator.emptyString }
                        Button{ text:(parent.dribble ? qsTr("true") : qsTr("false")) +translator.emptyString;width:parent.itemWidth
                            onClicked: {
                                parent.dribble = !parent.dribble;
                                serial.updateCommandParams(crazyShow.robotID,crazyShow.velX,crazyShow.velY,crazyShow.velR,crazyShow.dribble,crazyShow.mode,crazyShow.shoot,crazyShow.power);
                            }
                        }
                        Text{ text:qsTr("vel-y ") + translator.emptyString}
                        SpinBox{ minimumValue:-crazyShow.m_VEL; maximumValue:crazyShow.m_VEL; value:parent.velY;width:parent.itemWidth
                            onEditingFinished:{parent.velY = value;}}
                        Text{ text:qsTr("shoot") + translator.emptyString}
                        Button{ text:(parent.shoot? qsTr("true") : qsTr("false")) + translator.emptyString;width:parent.itemWidth
                            onClicked: {
                                parent.shoot = !parent.shoot;
                                serial.updateCommandParams(crazyShow.robotID,crazyShow.velX,crazyShow.velY,crazyShow.velR,crazyShow.dribble,crazyShow.mode,crazyShow.shoot,crazyShow.power);
                            }
                        }
                        Text{ text:qsTr("vel-r")  + translator.emptyString}
                        SpinBox{ minimumValue:-crazyShow.m_VELR; maximumValue:crazyShow.m_VELR; value:parent.velR;width:parent.itemWidth
                            onEditingFinished:{parent.velR = value;}}
                        Text{ text:qsTr("mode")  + translator.emptyString}
                        Button{ text:(parent.mode?qsTr("lift"):qsTr("flat")) + translator.emptyString;width:parent.itemWidth
                            onClicked: {
                                parent.mode = !parent.mode
                                serial.updateCommandParams(crazyShow.robotID,crazyShow.velX,crazyShow.velY,crazyShow.velR,crazyShow.dribble,crazyShow.mode,crazyShow.shoot,crazyShow.power);
                            }
                        }
                        Text{ text:qsTr("step") + translator.emptyString }
                        SpinBox{ minimumValue:1; maximumValue:crazyShow.m_VEL; value:parent.velStep;width:parent.itemWidth
                            onEditingFinished:{parent.velStep = value;}}
                        Text{ text:qsTr("power") + translator.emptyString }
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
                        text:qsTr("Start") + translator.emptyString;
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
                            text = (ifStarted ? qsTr("Stop") : qsTr("Start")) + translator.emptyString;
                        }
                    }
                }

            }
            Tab{
                anchors.fill: parent;
                property string title: qsTr("Referee") + translator.emptyString;
                RefereeBox{}
            }
            Tab{
                anchors.fill: parent;
                property string title: qsTr("Vision") + translator.emptyString;
                Rectangle{
                    id:vision;
                    anchors.top: parent.top;
                    anchors.topMargin: 10;
                    color : "lightgrey";
                    Column{
                        anchors.horizontalCenter: parent.horizontalCenter;
                        Grid{
                            id : controlGrid;
                            columns:3;
                            columnSpacing: 20;
                            rowSpacing: 5;
                            Text{ text:qsTr("Small") + translator.emptyString; }
                            Switch{
                                id:fieldOption;
                                style: SwitchStyle {
                                    groove: Rectangle {
                                         implicitWidth: 100
                                         implicitHeight: 20
                                         color:"black";
                                         border.width: 1
                                    }
                                }
                                checked: true;
                                onClicked:{
                                    fieldCanvas.ifBig = fieldOption.checked;
                                    interaction.fieldChange(fieldOption.checked);
                                }
                            }
                            Text{ text:qsTr("Big") + translator.emptyString; }
                        }
                    }
                    function autoSizeForListView(item){
                        var root = item.visibleChildren[0];
                        var listViewHeight = 0;
                        var listViewWidth = 0;
                        for (var i = 0; i < root.visibleChildren.length; i++) {
                            listViewHeight += root.visibleChildren[i].height;
                            listViewWidth  = Math.max(listViewWidth, root.visibleChildren[i].width);
                        }
                        item.height = listViewHeight;
                        item.width = listViewWidth;
                    }
                }
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
                    placeholderText: qsTr("Enter Command") + translator.emptyString;
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
    Component.onCompleted: {
        translator.selectLanguage("zh");
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
                text: modelData;//serial.getName(itemIndex);
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


