type action =
  | Nothing
  | DragEnter
  | DragLeave
  | DragEnd
  | DragStart
  | DragDrop(int, int);

let handleDragStart = (id, flag, dragImg, event) => {
  DragEventBaseUtils.dragStart(id, flag, dragImg, event);
  DragStart
};

let handleDragEnter = (id, handleFlag, handleRelationError, _event) => {
  DragEventBaseUtils.isTriggerDragEnter(id, handleFlag, handleRelationError) ? DragEnter : Nothing
};

let handleDragLeave = (id, handleFlag, handleRelationError, event) => {
  let e = ReactEvent.convertReactMouseEventToJsEvent(event);
  DomHelper.stopPropagation(e);
  DragEventBaseUtils.isTriggerDragLeave(id, handleFlag, handleRelationError, event) ? DragLeave : Nothing
};

let handleDragOver = (event) => {
  let e = ReactEvent.convertReactMouseEventToJsEvent(event);
  DomHelper.preventDefault(e)
};

let handleDrop = (uid, handleRelationError, event) => {
  let e = ReactEvent.convertReactMouseEventToJsEvent(event);
  let startId = DragUtils.getDragedUid(e);
  DragEventBaseUtils.isTriggerDragDrop(uid, startId, handleRelationError) ?
    DragLeave : DragDrop(uid, startId)
};

let handleDrageEnd = (_event) => {
  CurrentDragSourceEditorService.clearCurrentDragSource |> StateLogicService.getAndSetEditorState;
  DragEnd
};