open Js.Promise;

module CustomEventHandler = {
  include EmptyEventHandler.EmptyEventHandler;

  type prepareTuple = (Wonderjs.MaterialType.material, int);
  type dataTuple = int;
  type return = Js.Promise.t(unit);

  let _handleSetMap = (materialComponent, textureComponent, engineState) =>
    switch (
      LightMaterialEngineService.getLightMaterialDiffuseMap(
        materialComponent,
        engineState,
      )
    ) {
    | None =>
      OperateTextureLogicService.handleMaterialComponentFromNoMapToHasMap(
        (materialComponent, textureComponent),
        (
          LightMaterialEngineService.setLightMaterialDiffuseMap,
          LightMaterialEngineService.reInitLightMaterialsAndClearShaderCache,
        ),
        engineState,
      )
    | Some(_map) =>
      OperateTextureLogicService.changeTextureMapAndRefreshEngineState(
        materialComponent,
        textureComponent,
        LightMaterialEngineService.setLightMaterialDiffuseMap,
        engineState,
      )
    };

  let handleSelfLogic =
      (
        (uiState, dispatchFunc),
        (materialComponent, currentNodeId),
        draggedNodeId,
      ) => {
    MaterialDragTextureEventHandlerUtils.handleSelfLogic(
      (uiState, dispatchFunc),
      materialComponent,
      draggedNodeId,
      _handleSetMap,
    );

    LightMaterialForAssetEventHandlerUtils.createImgCanvasSnapshotAfterUpdateInspector(
      currentNodeId,
      dispatchFunc,
    );
  };
};

module MakeEventHandler = EventHandler.MakeEventHandler(CustomEventHandler);