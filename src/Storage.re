let saveToLocalStorage = (key, data) => Dom.Storage.setItem(key, data, Dom.Storage.localStorage);

let loadFromLocalStorage = key => Dom.Storage.getItem(key, Dom.Storage.localStorage);
