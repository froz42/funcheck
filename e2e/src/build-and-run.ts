import { Builder, BuilderProperty } from './builder';
import { Runner } from './runner';
import * as fs from 'fs';

export const OUTPUT_NAME = './output';

const FUNCHECK_PATH = '../host/funcheck';
const JSON_OPTION = '--json-output';

const DEFAULT_BUILDER_OPTS: BuilderProperty = {
  cc: 'gcc',
  flags: ['-g', '-Wall', '-Wextra', '-Werror'],
};

/**
 *
 * @param sources - The source files to build and run
 *
 * @returns The output of the program
 */
export default function buildAndRun(
  sources: string[],
  {
    builderProperty = DEFAULT_BUILDER_OPTS,
    funcheckOpts = [],
  }: {
    builderProperty?: BuilderProperty;
    funcheckOpts?: string[];
  }
): string {
  const builder = new Builder(builderProperty);
  builder.build(sources, OUTPUT_NAME);
  const runner = new Runner();
  const output = runner.run(FUNCHECK_PATH, [
    JSON_OPTION,
    ...funcheckOpts,
    OUTPUT_NAME,
  ]);
  fs.unlinkSync(OUTPUT_NAME);
  return output;
}
