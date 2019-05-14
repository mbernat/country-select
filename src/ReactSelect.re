[@bs.deriving {jsConverter: newType}]
type option = { value: string, label: string };

type valueType = int;
type actionMeta = int;

[@bs.deriving {jsConverter: newType}]
type filterConfig = {
  ignoreCase: bool,
  ignoreAccents: bool
};

type filter;

[@bs.module "react-select"]
external create_filter : abs_filterConfig => filter = "createFilter";

type components('a) = {
  .
  "Option": 'a
}

/*

export type OptionType = {
  [string]: any,
};

export type OptionsType = Array<OptionType>;

export type GroupType = {
  options: OptionsType,
  [string]: any,
};

export type ValueType = OptionType | OptionsType | null | void;

export type ActionTypes =
  | 'select-option'
  | 'deselect-option'
  | 'remove-value'
  | 'pop-value'
  | 'set-value'
  | 'clear'
  | 'create-option';

export type ActionMeta = {
  action: ActionTypes,
};
*/

module Option = {
  [@bs.module "react-select/lib/components/Option"][@react.component]
  external make: (~data: 'a) => React.element = "default";
}

module External = {
    [@bs.module "react-select"][@react.component]
    external make: (
        ~autoFocus: bool=?,
        ~className: string=?,
        ~components: 'a=?,
        ~controlShouldRenderValue: bool=?,
        ~filterOption: filter=?,
        ~menuIsOpen: bool=?,
        ~onChange: (abs_option, actionMeta) => unit=?,
        ~options: array(abs_option),
        ~placeholder: string=?,
        ~value: abs_option=?,
    ) => React.element = "default";
}

[@react.component]
let make = (
      ~autoFocus=?,
      ~className=?,
      ~components=?,
      ~controlShouldRenderValue=?,
      ~filterConfig=?,
      ~menuIsOpen=?,
      ~onChange=?,
      ~options, 
      ~placeholder=?,
      ~value=?
    ) => {
      let jsValue = Belt.Option.map(value, optionToJs);
      let filterOption = Belt.Option.map(filterConfig, fc => fc |> filterConfigToJs |> create_filter);
      <External
          ?autoFocus 
          ?className
          ?components
          ?controlShouldRenderValue
          value=?jsValue
          filterOption=?filterOption
          ?menuIsOpen
          ?onChange
          options=Array.map(optionToJs, options)
          ?placeholder
      />
    }