import { expect } from 'chai';
import * as sinon from 'sinon';

import { run } from './main';

describe('#main', () => {
  it(`should print to console and return true`, () => {
    // Spies / stubs
    const consoleSpy = sinon.spy(console, 'log');

    // Invocations
    const result = run();

    // Expectation
    expect(consoleSpy.calledWith('It begins!')).to.eql(true);
    expect(result).to.eql(true);
  });
});
