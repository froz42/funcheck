/* Funcheck - A tool for checking functions calls return protections
 * Copyright (C) 2023  Theo Matis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

import { Builder, BuilderProperty } from './builder';
import { Runner } from './runner';
import * as fs from 'fs';

export const OUTPUT_NAME = './output';

const FUNCHECK_PATH = '../host/funcheck';
const JSON_OPTION = '--json-output';

const DEFAULT_BUILDER_OPTS: BuilderProperty = {
  cc: 'clang',
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
