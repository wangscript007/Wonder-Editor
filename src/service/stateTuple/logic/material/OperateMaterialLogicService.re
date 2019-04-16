open MaterialDataAssetType;

let getDefaultSnapshotBase64 = () => "
data:image/jpeg;base64,/9j/4AAQSkZJRgABAQAAAQABAAD/2wBDAAEBAQEBAQEBAQEBAQEBAQIBAQEBAQIBAQECAgICAgICAgIDAwQDAwMDAwICAwQDAwQEBAQEAgMFBQQEBQQEBAT/2wBDAQEBAQEBAQIBAQIEAwIDBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAT/wAARCAAyADIDASIAAhEBAxEB/8QAHgABAAIBBAMAAAAAAAAAAAAAAAkKBwEGCAsDBAX/xAA3EAABAwMCAwQHBgcAAAAAAAABAgMEAAUGBxEIEiEJExQxIiNBVHGT0QpCUWFigRUkQ1Ox4fH/xAAUAQEAAAAAAAAAAAAAAAAAAAAA/8QAFBEBAAAAAAAAAAAAAAAAAAAAAP/aAAwDAQACEQMRAD8AivqQDgl7NniR46Lq5J03s0LFNMLZcDbsk1izVt2Ph9udQAp2LAaQO/uUtII3jxtkIK0h19ncb4t4MOGyVxS68Yxpw89Jg4hGWL/qDd4ii1JhWhhxCXWWF/dkSlqRHaV90uLX/TrsLNAcPxHTzTTGMFweyW3G8Wxa0MWaw2G1MhiDbIrCAhtpCfM+W6lq3UtSlKUVKUSQhe0t+zpcJmN2xg6ralazaqX/ALvllO2y8Q9NMaUsjqpiHFZdlJAPUByW55dd63LnP2ePgkyC1OMYXlGuenV5S2oRrnEzmPmEIrO/Kp+HcIjnOB09Fpxrce3frU9VKChTxz9jpxJ8Glqu2olqeia46HWpJkXPULEbW5bL/h7HMsd7kFhUt1xhlISCqbFckR08wLhYBFRG/wDQR1BrtP5MdmUy8xIabfYfbUy6y6gPNOoUClSVJIIIIJBBBBBIIIqjL2z3Z8Wzg91ltWpuldoFu0C1vnSn7NZ4yCYWnWRNAybjZG+p5IkhtSpsJBICEplMJARHb3CFqlKUE53Y9ot1hazvI3EoTcrtkLFtLpHrEx4UdKkIB/DvJLqvjtVsrSTPY/h4ja3QULQEqHN0qkZ2fOrDWIXm94y8/wByXZqLqwkr5QtLiEtqIH5Ka6/EVZb0j1iZciwiJYO6U7Ar3oJw2Hm5DSHmVBbbg5kqB3Bry1w2wbW1Mdptp19DzJ23Q4rcD4Vm+Pq7YnkJJQUqPXo4Cmgy1UTHbY4LaM+7PHWxucyl+7YGm2aoY4oAlyJKs9yjd6tIAJ9KJJntH9Lyt9h1rn3c9XLe02ssqbbATuDzekP3qGjtbuIeLb+ELWOz/wAQSmXmNjawqCz3g5n3LlMjsqSkHff1QfUfyQfLzoKXhTsSNx0O3tpWhO5JHkTuKUH1sfyW7Ybe7fk9lU4qbbVlTkZo7LnsEjvGR+o7BSf1JA9pqZbh14trZerVb3WrohW6U86S5yrSfIpUD1BBBBB6ggg1ClXox03ix3FV7xK7OWO5uK55TJSXbTc1e0vtDYpWf7rfpH2hVBbywziJiutMFM9J5kgj1v8Aus5wuISOlA/n09B5B0VUTxXir1Oxptpi8Y9PmdyQgSrJLbuLLg/HlJS4P3TWWUccGXONLRFx3IQ7tygzEtwGevt5lr/wDQWZ8t4n4NthvvO3NpKUJPVT4A6D41XX48OKuVr1lUHE7LOU/huJTFy3Xm3eZi8XEpU1zj8UMIU4lJ9qnVkeQrjJm+v+pOesOQ7jdF2y3Ogpcg299ZW8k+aXHuhIPkQkAfmawvQKUpQbw8JE92j/ACU/SnhInu0f5KfpSlA8JE92j/JT9KeEie7R/kp+lKUDwkT3aP8AJT9KeEie7R/kp+lKUDwkT3aP8lP0pSlB/9k=
";

let getNewMaterilaName = () => "New Material";

let getDefaultName = () => "NoName Material";

let _getName = (material, engineState, getMaterialNameFunc) =>
  switch (getMaterialNameFunc(material, engineState)) {
  | None => getDefaultName()
  | Some(name) => name
  };

let getName = (~material, ~type_, ~engineState) =>
  switch (type_) {
  | BasicMaterial =>
    _getName(
      material,
      engineState,
      BasicMaterialEngineService.getBasicMaterialName,
    )
  | LightMaterial =>
    _getName(
      material,
      engineState,
      LightMaterialEngineService.getLightMaterialName,
    )
  };

let setName = (~material, ~type_, ~name, ~engineState) =>
  switch (type_) {
  | BasicMaterial =>
    BasicMaterialEngineService.setBasicMaterialName(
      name,
      material,
      engineState,
    )
  | LightMaterial =>
    LightMaterialEngineService.setLightMaterialName(
      name,
      material,
      engineState,
    )
  };