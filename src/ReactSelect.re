[@bs.deriving {jsConverter: newType}]
type option = { value: string, label: string };

module External = {
    [@bs.module "react-select"][@react.component]
    external make: (~options: array(abs_option)) => React.element = "default";
}

[@react.component]
let make = (~options) =>
    <External options=Array.map(optionToJs, options) />