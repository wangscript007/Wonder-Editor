open CurrentNodeDataType;

module Method = {
  let onSelect = (fileId, fileType, dispatchFunc, _event) => {
    StateAssetService.getState()
    |> CurrentNodeDataAssetService.clearCurrentNodeData
    |> CurrentNodeDataAssetService.setCurrentNodeData({
         currentNodeId: fileId,
         nodeType: fileType,
       })
    |> StateAssetService.setState
    |> ignore;

    StateEditorService.getState()
    |> SceneEditorService.clearCurrentSceneTreeNode
    |> CurrentSelectSourceEditorService.setCurrentSelectSource(
         EditorType.AssetTree,
       )
    |> StateEditorService.setState
    |> ignore;

    dispatchFunc(AppStore.ReLoad) |> ignore;
  };
};

let component = ReasonReact.statelessComponent("FileBox");

let render = ((_store, dispatchFunc), attributeTuple, _self) => {
  let (dragImg, imgSrc, fileId, fileType, fileName, flag, isSelected) = attributeTuple;
  let className = "wonder-asset-fileBox " ++ (isSelected ? "item-active" : "");
  <article
    className
    onClick=(
      _event => Method.onSelect(fileId, fileType, dispatchFunc, _event)
    )>
    <img
      src=imgSrc
      onDragStart=(DragEventBaseUtils.dragStart(fileId, flag, dragImg))
    />
    <span className="item-text"> (DomHelper.textEl(fileName)) </span>
  </article>;
};

let make = (~store, ~dispatchFunc, ~attributeTuple, _children) => {
  ...component,
  render: self => render((store, dispatchFunc), attributeTuple, self),
};