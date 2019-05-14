[%raw {|require('../../../node_modules/flag-icon-css/css/flag-icon.min.css')|}]

module Decode = {
    open Json.Decode

    let option = option =>
        ReactSelect.{
            value: field("value", string, option),
            label: field("label", string, option)
        }
    
    let options = json: array(ReactSelect.option) => Json.Decode.array(option, json)
}

let fetchOptions = (url) =>
    Js.Promise.(
        Fetch.fetch(url)
        |> then_(Fetch.Response.json)
        |> then_(json => json |> Decode.options |> (options => Some(options)) |> resolve)
        |> catch(err => { Js.log(err); resolve(None)})
    )

let countryUrl = "https://gist.githubusercontent.com/rusty-key/659db3f4566df459bd59c8a53dc9f71f/raw/4127f9550ef063121c564025f6d27dceeb279623/counties.json";

let findValue(options, value) = Belt.Array.getBy(options, o => o.ReactSelect.value == value);

module Option = {
    let make = (props) => {
        let className = "flag-icon flag-icon-" ++ props##data##value;
        <div className="option">
            <span className=className></span>
            {ReactSelect.Option.make(props)}
        </div>
    }
}

[@react.component]
let make = (~className, ~country, ~onChange) => {
    let (isOpen, setIsOpen) = React.useState(() => false);
    let toggleOpen = _ => setIsOpen(isOpen => !isOpen);
    let (value, setValue) = React.useState(() => None);
    let (options, setOptions) = React.useState(() => [||]);
    // TODO loading/error handling
    React.useEffect1(() => {
        Js.Promise.(
            fetchOptions(countryUrl)
            |> then_(result => switch(result) {
                | Some(options) => {
                    setOptions(_ => options);
                    switch(country) {
                    | Some(country) => setValue(_ => findValue(options, country))
                    | None => ()
                    }
                    toggleOpen()
                }
                | None => ()
             } |> resolve)
            |> ignore
        )
        None
    }, [||])

    let jsChange = (jsOption, _action) => {
        // TODO check that _action was 'select option'
        let option = ReactSelect.optionFromJs(jsOption);
        setValue(_ => Some(option))
        toggleOpen()
        onChange(option.value);
    };

    let filterConfig = ReactSelect.{
        ignoreCase: true,
        ignoreAccents: false
    };

    let select =
        <ReactSelect
            autoFocus=true
            className=className
            components={"Option": Option.make, "IndicatorSeparator": Js.null, "DropdownIndicator": Js.null}
            controlShouldRenderValue=false
            filterConfig=filterConfig
            menuIsOpen=true
            onChange=jsChange
            options=options
            placeholder="Search"
            value=?value
        />;

    <div className={className} >
        <button
            //iconAfter={<ChevronDown />}
            onClick=toggleOpen
            //isSelected={isOpen}
        >
            {switch(value) {
                | Some(value) => React.string(value.ReactSelect.label)
                | None => React.string("Select country")
            }}
        </button>
        {isOpen ? <div className="menu">select</div> : React.null}
        {isOpen ? <div className="blanket" onClick={toggleOpen} /> : React.null}
    </div>
}