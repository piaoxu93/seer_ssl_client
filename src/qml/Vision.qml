import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.5
import QtQuick.Controls.Styles 1.4
import Client.Component 1.0 as Client
Item {
    id:vision;
    anchors.top: parent.top;
    anchors.topMargin: 10;
    Client.Interaction{
        id : interaction;
    }
    Column{
        anchors.horizontalCenter: parent.horizontalCenter;
        Grid{
            id : controlGrid;
            columns:2;
            columnSpacing: 20;
            rowSpacing: 5;
            Repeater{
                model:interaction.getCameraNumber();
                CheckBox{
                    text:"Camera" + index;
                    checked: true;
                    onClicked:{
                        interaction.visionControlChanged(index);
                    }
                }
            }
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