
[%raw {|require('../../../src/style.css')|}]

module Example = {
    [@react.component]
    let make = () => {
        <CountrySelect
            className="custom-class"
            country=Some("us")
            onChange=(country => Js.log(country))
        />
    }
}

ReactDOMRe.renderToElementWithId(<Example />, "example");
