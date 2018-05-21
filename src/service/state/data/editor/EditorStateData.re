open EditorType;

open EditorStateDataType;

let _createState = () => {
  assetRecord: {
    assetTree: None,
    index: 0,
    currentAssetTreeNode: None,
    currentAssetFileNode: None,
    folderArray: [||],
    fileMap: WonderCommonlib.SparseMapService.createEmpty()
  },
  sceneRecord: {root: None, diffMap: None, currentSceneTreeNode: None, isRun: false},
  currentSign: "",
  currentSource: None,
  loopId: (-1)
};

let editorStateData = {
  state: _createState(),
  isDebug: true,
  engineStateDataForEdit: StateEngineService.createStateData(),
  engineStateDataForRun: StateEngineService.createStateData()
};