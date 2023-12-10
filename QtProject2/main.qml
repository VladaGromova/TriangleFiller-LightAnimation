import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
Window {
    id: mainWindow
    visible : true
    width : 1100
    height : 540
    title : qsTr("Triangle Filling")
    color : "powderblue"
    maximumHeight: height
    maximumWidth: width

    minimumHeight: height
    minimumWidth: width
     Timer {
        id: timer
        interval: 100
        running: true 
        repeat: true 
    
        onTriggered: {
            if (windowController.anim) {
                windowController.moveLight();
            }
        }
    }
    Connections {
        target : windowController
        function onImageChanged() {
            image.reload()
        }
        function onGridShowed() {
            image.reload()
        }
        function onGridHiden() {
            image.reload()
        }
        function onControlPointsShowed() {
            image.reload()
        }
        function onControlPointsHiden() {
            image.reload()
        }
        function onGridAccuracySetted() {
            image.reload()
        }
        function onKdSetted() {
            image.reload()
        }
        function onKsSetted() {
            image.reload()
        }
        function onMSetted() {
            image.reload()
        }
        function onBackgroundColorChanged() {
            image.reload()
        }
        function onLightColorChanged() {
            image.reload()
        }
        function onLightHeightSetted() {
            image.reload()
        }
        function onZChoosed() {
            image.reload()
        }
        function onZValueSetted() {
            image.reload()
        }
        function onBackgroundImageSetted() {
            image.reload()
        }
        function onNormalMapModified(){
            image.reload()
        }
        function onNormalMapRemoved(){
            image.reload()
        }
        function onNormalMapSetted(){
            image.reload()
        }
        function onLightMoved(){
            image.reload()
        }
    }
    GridLayout {
        id: mainGrid
        anchors.fill : parent
        columns : 2
        rows : 1
        height: mainWindow.height
        Rectangle {
            objectName : "gridRectangle"
            Layout.column : 0
            Layout.row : 0
            Layout.fillHeight : true
            Layout.fillWidth : true
            color : "powderblue"
            height: mainWindow.height
            Image {
                id : image
                x : 10
                y : 10
                width : 512 
                height : 512 
                source : "image://ImageSource/image"
                cache : false
                focus : true
                MouseArea {
                    anchors.fill : parent
                    onClicked : {
                        windowController.chooseZ(mouseX / image.width, mouseY / image.height)
                    }
                }
                function reload() {
                    var oldSource = source
                    source = ""
                    source = oldSource
                }
            } 
        } 
        Rectangle {
            Layout.column : 1
            Layout.row : 0
            Layout.fillHeight : true
            Layout.fillWidth : true
            color : "powderblue"
            Layout.bottomMargin : 30
                ColumnLayout {
                    anchors.fill : parent
                    GroupBox {
                        title : qsTr("Color settings")
                        Layout.fillWidth : true
                        Layout.fillHeight : false
                        width : parent.width
                        RowLayout {
                            spacing : 30
                            ColumnLayout {
                                spacing : 15
                                RadioButton {
                                    id : radioButtonColor
                                    text : "Background as color"
                                    onCheckedChanged : {
                                        if (checked) {
                                            colorPickerBackground.open()
                                        }
                                    }
                                }
                                RadioButton {
                                    id : radioButtonImage
                                    checked : true
                                    text : "Background as image"
                                    onCheckedChanged : {
                                        if (checked) {
                                            fileDialogImage.visible = true
                                        }
                                    }
                                }
                                Text {
                                    text : "Choose color of the light"
                                }
                            }
                            ColumnLayout {
                                ColorDialog {
                                    id : colorPickerBackground
                                    title : "Select background color"
                                    onAccepted : {
                                        windowController.changeBackgroundColor(colorPickerBackground.selectedColor)
                                    }
                                }
                                Button {
                                    text : "Color palette..."
                                    enabled : radioButtonColor.checked
                                    onClicked : colorPickerBackground.open()
                                }
                                FileDialog {
                                    id : fileDialogImage
                                    title : "Choose a File"
                                    onAccepted : {
                                        windowController.setBackgroundImage(fileDialogImage.selectedFile)
                                    }
                                }
                                Button {
                                    text : "File..."
                                    enabled : radioButtonImage.checked
                                    onClicked : {
                                        fileDialogImage.visible = true
                                    }
                                }
                                ColorDialog {
                                    id : colorPickerLight
                                    title : "Select a color"
                                    onAccepted : {
                                        windowController.changeLightColor(colorPickerLight.selectedColor)
                                    }
                                }
                                Button {
                                    text : "Choose..."
                                    onClicked : colorPickerLight.open()
                                }
                            }
                        }
                    }
                    GroupBox {
                        title : qsTr("Parameters settings")
                        Layout.fillWidth : true
                        Layout.fillHeight : false
                        width : parent.width
                        RowLayout {
                            spacing : 40
                            ColumnLayout { 
                                Text {
                                    text : "Accuracy"
                                }
                                Text {
                                    text : "KD"
                                }
                                Text {
                                    text : "KS"
                                }
                                Text {
                                    text : "M"
                                }
                                CheckBox {
                                    text : "Show Grid"
                                    onCheckedChanged : {
                                        if (checked) {
                                            windowController.showGrid() 
                                        } else {
                                            windowController.hideGrid()
                                        }
                                    }
                                }
                                CheckBox {
                                    text : "Show Control Points"
                                    onCheckedChanged : {
                                        if (checked) {
                                            windowController.showControlPoints() 
                                        } else {
                                            windowController.hideControlPoints()
                                        }
                                    }
                                }
                            }
                            ColumnLayout { 
                                Slider {
                                    id : triangleGridSlider
                                    from : 1
                                    value : 2
                                    to : 5
                                    onValueChanged : {
                                        windowController.setGridAccuracy(value)
                                    }
                                }
                                Slider {
                                    id : kdSlider
                                    from : 0
                                    value : 0.3
                                    to : 1
                                    stepSize : 0.1
                                    onValueChanged : {
                                        windowController.setKd(value)
                                    }
                                }
                                Slider {
                                    id : ksSlider
                                    from : 0
                                    value : 0.6
                                    to : 1
                                    stepSize : 0.1
                                    onValueChanged : {
                                        windowController.setKs(value)
                                    }
                                }
                                Slider {
                                    id : mSlider
                                    from : 1
                                    value : 50
                                    to : 100
                                    stepSize : 10
                                    onValueChanged : {
                                        windowController.setM(value)
                                    }
                                }
                                Text {
                                    text : ""
                                }
                                Text {
                                    text : ""
                                }
                            }
                        }
                    }
                    GroupBox {
                        title : "Height settings"
                        Layout.fillWidth : true
                        Layout.fillHeight : false
                        width : parent.width
                        RowLayout {
                            spacing : 20
                            ColumnLayout {
                                Text {
                                    text : "Change z of control points"
                                }
                                Text {
                                    text : "Light height"
                                }
                            }
                            ColumnLayout {
                                Slider {
                                    id : zSlider
                                    from : 0.0
                                    to : 10.0
                                    onValueChanged: {
                                        windowController.setZValue(zSlider.value)
                                    }
                                }
                                Slider {
                                    id : lightHeightSlider
                                    from : 0.0
                                    value : 2.0
                                    to : 10.0
                                    onValueChanged : {
                                        windowController.setLightHeight(value)
                                    }
                                }
                            }
                        }
                    }
                    GroupBox {
                        title : "Normal map settings"
                        Layout.fillWidth : true
                        Layout.fillHeight : false
                        width : parent.width
                        RowLayout {
                            spacing : 30
                            ColumnLayout {
                                spacing : 15
                                RadioButton {
                                    text : "Default normal map"
                                    checked: true
                                    onCheckedChanged : {
                                        if (checked) {
                                            windowController.setDefaultNormalMap()
                                        }
                                    }
                                }
                                FileDialog {
                                    id : normalMapDialog
                                    title : "Choose a File"
                                    onAccepted : {
                                        windowController.setNormalMap(normalMapDialog.selectedFile)
                                    }
                                }
                                RadioButton {
                                    text : "Uploaded normal map"
                                    onCheckedChanged : {
                                        if (checked) {
                                            normalMapDialog.visible = true;
                                        }
                                    }
                                }
                                CheckBox{
                                    id: mustBeModifiedCheckbox
                                    text : "Modify normal map"
                                    onCheckedChanged : {
                                        if (checked) {
                                            windowController.modifyNormalMap() 
                                        } else {
                                            windowController.removeNormalMap()
                                        }
                                    }
                                }
                            }
                            ColumnLayout {
                                spacing : 15
                                Text{
                                    text: ""
                                }
                                Button {
                                    text : "Upload"
                                    onClicked : {
                                        normalMapDialog.visible = true
                                    }
                                }
                                Text{
                                    text: ""
                                }
                            }
                        }
                    }
                    
                    RowLayout{
                        Button {
                            id: animateButton
                            text : "Animate"
                            onClicked : {
                                windowController.animate()
                            }
                            background: Rectangle {
                                color: "lightgreen" 
                                border.color: "darkgreen"
                                border.width: 1
                                radius: 5
                            }
                        }
                        Button {
                            id: stopButton
                            text : "Stop"
                            onClicked : {
                                windowController.stopAnimation();
                            }
                            background: Rectangle {
                                color: "lightcoral" 
                                border.color: "darkred"
                                border.width: 1 
                                radius: 5
                            }
                        }
                    }
                }
        }
    }
}