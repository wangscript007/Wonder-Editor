open Js.Typed_array;

/* let _buildEmptyUint8Array = () => Uint8Array.make([||]); */

let _getUint8Array = (uint8Array, base64, editorState) =>
  /* switch (uint8Array) {
     | Some(uint8Array) => uint8Array
     | None =>
       switch (base64) {
       | Some(base64) => BufferUtils.convertBase64ToUint8Array(base64)
       | None =>
         ConsoleUtils.error(
           LogUtils.buildErrorMessage(
             ~description={j|image->base64 should exist|j},
             ~reason="",
             ~solution={j||j},
             ~params={j||j},
           ),
           editorState,
         );

         _buildEmptyUint8Array();
       }
     }; */
  uint8Array |> OptionService.unsafeGet;

let _computeBufferViewDataByteLength = bufferViewArr =>
  switch (bufferViewArr |> ArrayService.getLast) {
  | None => 0
  | Some(({byteOffset, byteLength}: ExportAssetType.bufferView)) =>
    byteOffset + BufferUtils.alignedLength(byteLength)
  };

let _buildImageData = editorState => {
  let (imageIndexMap, imageArr, bufferViewArr, uint8ArrayArr, byteOffset) =
    ImageNodeMapAssetEditorService.getImageNodeMap(editorState)
    /* |> SparseMapService.filter(({isInWDB}: AssetNodeType.imageResultType) =>
         ! isInWDB
       ) */
    |> SparseMapService.reduceiValid(
         (.
           (
             imageIndexMap,
             imageArr,
             bufferViewArr,
             uint8ArrayArr,
             byteOffset,
           ),
           {name, mimeType, uint8Array, base64, isInWDB}: AssetNodeType.imageResultType,
           imageNodeId,
         ) =>
           isInWDB ?
             (
               imageIndexMap,
               imageArr,
               bufferViewArr,
               uint8ArrayArr,
               byteOffset,
             ) :
             {
               let uint8Array =
                 _getUint8Array(uint8Array, base64, editorState);
               let byteLength = uint8Array |> Uint8Array.length;
               let alignedByteLength = BufferUtils.alignedLength(byteLength);

               (
                 imageIndexMap
                 |> WonderCommonlib.SparseMapService.set(
                      imageNodeId,
                      imageArr |> Js.Array.length,
                    ),
                 imageArr
                 |> ArrayService.push(
                      {
                        name,
                        mimeType,
                        bufferView: bufferViewArr |> Js.Array.length,
                      }: ExportAssetType.image,
                    ),
                 bufferViewArr
                 |> ArrayService.push(
                      {byteOffset, byteLength}: ExportAssetType.bufferView,
                    ),
                 uint8ArrayArr |> ArrayService.push(uint8Array),
                 byteOffset + alignedByteLength,
               );
             },
         (
           WonderCommonlib.SparseMapService.createEmpty(),
           [||],
           [||],
           [||],
           0,
         ),
       );

  (
    imageIndexMap,
    imageArr,
    bufferViewArr,
    uint8ArrayArr,
    _computeBufferViewDataByteLength(bufferViewArr),
  );
};

let rec _getAssetNodePathFromAssets =
        (parentFolderNodeId, namePathArr, (editorState, engineState)) =>
  switch (parentFolderNodeId) {
  | None => namePathArr |> Js.Array.reverseInPlace |> Js.Array.joinWith("/")
  | Some(parentFolderNodeId) =>
    _getAssetNodePathFromAssets(
      AssetNodeUtils.getAssetNodeParentId(
        Folder,
        parentFolderNodeId,
        editorState,
      ),
      namePathArr
      |> Js.Array.copy
      |> ArrayService.push(
           AssetNodeUtils.getAssetNodeTotalName(
             Folder,
             parentFolderNodeId,
             (editorState, engineState),
           ),
         ),
      (editorState, engineState),
    )
  };

let _buildTextureData = (imageIndexMap, (editorState, engineState)) =>
  TextureNodeMapAssetEditorService.getTextureNodeMap(editorState)
  |> SparseMapService.reduceiValid(
       (.
         (textureIndexMap, textureArr),
         {textureComponent, image, parentFolderNodeId, isInWDB}: AssetNodeType.textureResultType,
         textureNodeId,
       ) =>
         isInWDB ?
           (textureIndexMap, textureArr) :
           (
             textureIndexMap
             |> WonderCommonlib.SparseMapService.set(
                  textureComponent,
                  textureArr |> Js.Array.length,
                ),
             textureArr
             |> ArrayService.push(
                  {
                    path:
                      _getAssetNodePathFromAssets(
                        parentFolderNodeId,
                        [||],
                        (editorState, engineState),
                      ),
                    name:
                      AssetNodeUtils.getAssetNodeTotalName(
                        Texture,
                        textureNodeId,
                        (editorState, engineState),
                      ),
                    source:
                      imageIndexMap
                      |> WonderCommonlib.SparseMapService.unsafeGet(image),
                    wrapS:
                      BasicSourceTextureEngineService.getWrapS(
                        textureComponent,
                        engineState,
                      )
                      |> TextureTypeUtils.convertWrapToInt,
                    wrapT:
                      BasicSourceTextureEngineService.getWrapT(
                        textureComponent,
                        engineState,
                      )
                      |> TextureTypeUtils.convertWrapToInt,
                    minFilter:
                      BasicSourceTextureEngineService.getMinFilter(
                        textureComponent,
                        engineState,
                      )
                      |> TextureTypeUtils.convertFilterToInt,
                    magFilter:
                      BasicSourceTextureEngineService.getMagFilter(
                        textureComponent,
                        engineState,
                      )
                      |> TextureTypeUtils.convertFilterToInt,
                    format:
                      BasicSourceTextureEngineService.getFormat(
                        textureComponent,
                        engineState,
                      )
                      |> TextureTypeUtils.convertFormatToInt,
                    type_:
                      BasicSourceTextureEngineService.getType(
                        textureComponent,
                        engineState,
                      ),
                    flipY:
                      BasicSourceTextureEngineService.getFlipY(
                        textureComponent,
                        engineState,
                      ),
                  }: ExportAssetType.texture,
                ),
           ),
       (WonderCommonlib.SparseMapService.createEmpty(), [||]),
     );

let _getTextureIndexFromMap = (textureComponent, textureIndexMap) =>
  switch (textureComponent) {
  | None => None
  | Some(textureComponent) =>
    textureIndexMap
    |> WonderCommonlib.SparseMapService.unsafeGet(textureComponent)
    |. Some
  };

let _buildMaterialData = (textureIndexMap, (editorState, engineState)) =>
  MaterialNodeMapAssetEditorService.getMaterialNodeMap(editorState)
  /* |> SparseMapService.filter(({isInWDB}: AssetNodeType.materialResultType) =>
       ! isInWDB
     ) */
  |> SparseMapService.reduceiValid(
       (.
         (basicMaterialArr, lightMaterialArr),
         {materialComponent, type_, parentFolderNodeId, isInWDB}: AssetNodeType.materialResultType,
         materialNodeId,
       ) =>
         isInWDB ?
           (basicMaterialArr, lightMaterialArr) :
           {
             let name =
               AssetNodeUtils.getAssetNodeTotalName(
                 Material,
                 materialNodeId,
                 (editorState, engineState),
               );

             switch (type_) {
             | BasicMaterial => (
                 basicMaterialArr
                 |> ArrayService.push(
                      {
                        name,
                        path:
                          _getAssetNodePathFromAssets(
                            parentFolderNodeId,
                            [||],
                            (editorState, engineState),
                          ),
                        color:
                          BasicMaterialEngineService.getColor(
                            materialComponent,
                            engineState,
                          ),
                      }: ExportAssetType.basicMaterial,
                    ),
                 lightMaterialArr,
               )
             | LightMaterial => (
                 basicMaterialArr,
                 lightMaterialArr
                 |> ArrayService.push(
                      {
                        name,
                        path:
                          _getAssetNodePathFromAssets(
                            parentFolderNodeId,
                            [||],
                            (editorState, engineState),
                          ),
                        diffuseColor:
                          LightMaterialEngineService.getLightMaterialDiffuseColor(
                            materialComponent,
                            engineState,
                          ),
                        diffuseMap:
                          _getTextureIndexFromMap(
                            LightMaterialEngineService.getLightMaterialDiffuseMap(
                              materialComponent,
                              engineState,
                            ),
                            textureIndexMap,
                          ),
                        shininess:
                          LightMaterialEngineService.getLightMaterialShininess(
                            materialComponent,
                            engineState,
                          ),
                      }: ExportAssetType.lightMaterial,
                    ),
               )
             };
           },
       ([||], [||]),
     );

let _buildWDBData =
    (imageAlignedByteLength, imageBufferViewArr, (editorState, engineState)) => {
  let imageBufferViewIndex = imageBufferViewArr |> Js.Array.length;

  let (wdbArr, arrayBufferArr, bufferViewArr, byteOffset) =
    WDBNodeMapAssetEditorService.getWDBNodeMap(editorState)
    |> SparseMapService.reduceValid(
         (.
           (wdbArr, arrayBufferArr, bufferViewArr, byteOffset),
           {name, parentFolderNodeId, wdbArrayBuffer}: AssetNodeType.wdbResultType,
         ) => {
           let byteLength = wdbArrayBuffer |> ArrayBuffer.byteLength;
           let alignedByteLength = BufferUtils.alignedLength(byteLength);

           (
             wdbArr
             |> ArrayService.push(
                  {
                    name,
                    path:
                      _getAssetNodePathFromAssets(
                        parentFolderNodeId,
                        [||],
                        (editorState, engineState),
                      ),
                    bufferView:
                      imageBufferViewIndex + (bufferViewArr |> Js.Array.length),
                  }: ExportAssetType.wdb,
                ),
             arrayBufferArr |> ArrayService.push(wdbArrayBuffer),
             bufferViewArr
             |> ArrayService.push(
                  {byteOffset, byteLength}: ExportAssetType.bufferView,
                ),
             byteOffset + alignedByteLength,
           );
         },
         ([||], [||], [||], imageAlignedByteLength),
       );

  (
    wdbArr,
    arrayBufferArr,
    bufferViewArr,
    bufferViewArr |> Js.Array.length === 0 ?
      imageAlignedByteLength : _computeBufferViewDataByteLength(bufferViewArr),
  );
};

let buildJsonData = (editorState, engineState) => {
  let (
    imageIndexMap,
    imageArr,
    imageBufferViewArr,
    imageUint8ArrayArr,
    imageAlignedByteLength,
  ) =
    _buildImageData(editorState);

  WonderLog.Log.print((
    "aaa:",
    imageIndexMap,
    imageArr,
    imageBufferViewArr,
    imageUint8ArrayArr,
    imageAlignedByteLength,
  ))
  |> ignore;
  let (textureIndexMap, textureArr) =
    _buildTextureData(imageIndexMap, (editorState, engineState));
  let (basicMaterialArr, lightMaterialArr) =
    _buildMaterialData(textureIndexMap, (editorState, engineState));
  let (
    wdbArr,
    wdbArrayBufferArr,
    wdbBufferViewArr,
    bufferTotalAlignedByteLength,
  ) =
    _buildWDBData(
      imageAlignedByteLength,
      imageBufferViewArr,
      (editorState, engineState),
    );
  WonderLog.Log.print((
    "aaa:",
    wdbArr,
    wdbArrayBufferArr,
    wdbBufferViewArr,
    bufferTotalAlignedByteLength,
  ))
  |> ignore;

  (
    (imageArr, textureArr, basicMaterialArr, lightMaterialArr, wdbArr),
    (imageBufferViewArr, wdbBufferViewArr),
    (imageUint8ArrayArr, wdbArrayBufferArr),
    bufferTotalAlignedByteLength,
  );
};

let buildJsonUint8Array =
    (
      bufferAlignedByteLength,
      (
        bufferViewArr,
        imageArr,
        textureArr,
        basicMaterialArr,
        lightMaterialArr,
        wdbArr,
      ),
    ) => {
  let encoder = TextEncoder.newTextEncoder();

  encoder
  |> TextEncoder.encodeUint8Array(
       (
         {
           copyright: {
             version: Copyright.getVersion(),
             author: Copyright.getAuthor(),
           },
           images: imageArr,
           textures: textureArr,
           basicMaterials: basicMaterialArr,
           lightMaterials: lightMaterialArr,
           wdbs: wdbArr,
           bufferViews: bufferViewArr,
         }: ExportAssetType.assets
       )
       |> Obj.magic
       |> Js.Json.stringify,
     );
};